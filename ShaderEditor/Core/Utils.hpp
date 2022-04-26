//
//  Utils.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 21.04.2022.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <optional>
#include <utility>

namespace Core {
namespace Utils {


class FileDialogs
{
public:
    // To be difined by Platforms
    static std::string OpenFile(const char* filter);
    static std::string SaveFile(const char* filter);
};


class FileSystem
{
public:
    static std::optional<std::string> ReadFile(const std::string& filepath);
    static void WriteFile(const std::string& content, const std::string& filepath);
};

enum class FileStatus {
    None = 0,
    FileNotFound,
    FileAttached,
    FileDeleted,
    FileCreated,
    FileChanged,
    FileUnchanged
};

class File {
public:
    File(const std::filesystem::path& path) : m_Path(path) {
        EN_INFO("Loading file {0}", path.c_str());
    }
    ~File() {
        EN_WARN("Unloading file {0}", GetPath().c_str());
    }
    
    FileStatus GetStatus() { return m_FileStatus; }
    const std::filesystem::path& GetPath() { return m_Path; }
    
    std::optional<std::string> Read();
    void Write(const std::string& content);
    
private:
    friend class FileWatcher;
    
    void SetStatus(FileStatus status) { m_FileStatus = status;}
private:
    FileStatus m_FileStatus = FileStatus::None;
private:
    std::filesystem::path m_Path;
    std::filesystem::file_time_type m_ModificationTime;
    bool m_FileExist = false;
};

class FileWatcher {
public:
    FileWatcher();
    ~FileWatcher();
    
    void Begin();
    void End();

    std::shared_ptr<File> LoadFile(const std::filesystem::path& path);
    
    static FileWatcher& Get() { return *s_FileWatcher; }
private:
    std::pair<std::filesystem::file_time_type, bool> GetWriteTime(const std::filesystem::path& path);
    void UpdateStatus(const std::shared_ptr<File>& file);
private:
    static FileWatcher* s_FileWatcher;
    std::vector<std::shared_ptr<File>> m_ManagedFiles;
};

}}

#endif /* Utils_hpp */
