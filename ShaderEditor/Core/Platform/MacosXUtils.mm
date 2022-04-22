//
//  MacosXUtils.m
//  ShaderEditor
//
//  Created by Vitaly Cloud on 22.04.2022.
//
#include "Core/pch.h"
#include <AppKit/AppKit.h>

std::string Core::Utils::FileDialogs::OpenFile(const char* filter) {
    EN_CORE_WARN("Filter is not implemented!");
    std::string filePath;
    @autoreleasepool {
        // create an open documet panel
        NSOpenPanel *panel = [NSOpenPanel openPanel];
        [panel setCanChooseDirectories:NO];
        [panel setAllowsMultipleSelection:NO];
        
        
        NSModalResponse responce = [panel runModal];
        if(responce == NSModalResponseOK) {
            NSString *theString = [[panel URL]path];
            std::string path = std::string([theString UTF8String]);
            filePath.assign(path);
        }
    }
    return filePath;
}

std::string Core::Utils::FileDialogs::SaveFile(const char* filter) {
    EN_CORE_WARN("Filter is not implemented!");
//    EN_CORE_ERROR("SaveFile is not implemented in Macosx Platform");
    
    std::string filePath;
    @autoreleasepool {
        // create an open documet panel
        NSSavePanel *panel = [NSSavePanel savePanel];
        
        NSModalResponse responce = [panel runModal];
        if(responce == NSModalResponseOK) {
            NSString *theString = [[panel URL]path];
            std::string path = std::string([theString UTF8String]);
            filePath.assign(path);
        }
    }
    return filePath;
}
