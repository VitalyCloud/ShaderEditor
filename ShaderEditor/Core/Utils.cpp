//
//  Utils.cpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//
#include "Core/pch.h"

namespace Core {
namespace Utils {


std::optional<std::string> FileSystem::ReadFile(const std::string& filepath) {
    
    std::string result;
    // ifstream closes itself due to RAII
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    
    if(in) {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        
        if(size != -1) {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
        } else {
            EN_CORE_ERROR("Could not read from file: {0}", filepath);
            return {};
        }
    } else {
        EN_CORE_ERROR("Could not open file: {0}", filepath);
        return  {};
    }
    
    return result;
}


void FileSystem::WriteFile(const std::string& content, const std::string& filepath) {
    std::ofstream out(filepath);
    if(out.good()) {
        out << content;
        EN_INFO("File '{0}' saved", filepath);
    } else {
        EN_ERROR("File '{0}' is not saved", filepath);
    }
    out.close();
}

std::optional<std::string> File::Read() {
    return FileSystem::ReadFile(m_Path);
}

void File::Write(const std::string& content) {
    FileSystem::WriteFile(content, m_Path);
}

FileWatcher* FileWatcher::s_FileWatcher;

FileWatcher::FileWatcher() {
    EN_CORE_ASSERT(s_FileWatcher == nullptr, "File Watcher is alredy created");
    s_FileWatcher = this;
}

FileWatcher::~FileWatcher() {
    
}

void FileWatcher::Begin() {
    for(const auto& file: m_ManagedFiles) {
        UpdateStatus(file);
    }
}

void FileWatcher::End() {
    for(int i=0; i<m_ManagedFiles.size(); i++) {
        if(m_ManagedFiles[i].use_count() == 1)
            m_ManagedFiles.erase(m_ManagedFiles.begin() + i);
        auto status = m_ManagedFiles[i]->GetStatus();
        if(status != FileStatus::FileNotFound &&
           status != FileStatus::FileDeleted)
            m_ManagedFiles[i]->SetStatus(FileStatus::FileUnchanged);
    }
}

std::shared_ptr<File> FileWatcher::LoadFile(const std::filesystem::path& path) {
    for(int i=0; i<m_ManagedFiles.size(); i++) {
        auto file = m_ManagedFiles[i];
        if(file->GetPath() == path) {
            EN_INFO("Reuse loaded file {0}", file->GetPath().c_str());
            return file;
        }
    }
    
    auto file = Core::CreateRef<File>(path);
    m_ManagedFiles.push_back(file);
    UpdateStatus(file);
    return file;
}

std::pair<std::filesystem::file_time_type, bool> FileWatcher::GetWriteTime(const std::filesystem::path& path) {
    std::error_code error;
    auto time = std::filesystem::last_write_time(path, error);
    return {time, error.value() == 0};
}

void FileWatcher::UpdateStatus(const std::shared_ptr<File>& file) {
    auto [time, fileExist] = GetWriteTime(file->GetPath());
    
    if(file->GetStatus() == FileStatus::None) {
        // Start watching
        if(fileExist) {
            file->m_ModificationTime = time;
            file->SetStatus(FileStatus::FileAttached);
            file->m_FileExist = true;
        } else {
            file->SetStatus(FileStatus::FileNotFound);
            file->m_FileExist = false;
        }
        return;
    }
    
    if(!fileExist) {
        // File not exsit
        if(file->m_FileExist) {
            // Just deleted
            file->m_FileExist = false;
            file->SetStatus(FileStatus::FileDeleted);
        }
        // Have deleted for a while
        return;
    } else {
        // File Exist
        if(!file->m_FileExist) {
            // Just Created
            file->m_FileExist = true;
            file->SetStatus(FileStatus::FileCreated);
            return;
        }
        
        if(time == file->m_ModificationTime) {
            // Unchanged
            file->SetStatus(FileStatus::FileUnchanged);
        } else {
            // Modified
            file->m_ModificationTime = time;
            file->SetStatus(FileStatus::FileChanged);
        }
    }
}

}}
