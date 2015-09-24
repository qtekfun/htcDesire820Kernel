// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_FILE_UTIL_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_FILE_UTIL_H_
#pragma once

#include <string>
#include <map>

#include "chrome/common/extensions/extension.h"

class Extension;
class ExtensionMessageBundle;
class FilePath;
class GURL;

namespace extension_file_util {

extern const char kInstallDirectoryName[];

FilePath InstallExtension(const FilePath& unpacked_source_dir,
                          const std::string& id,
                          const std::string& version,
                          const FilePath& extensions_dir);

void UninstallExtension(const FilePath& extensions_dir,
                        const std::string& id);

scoped_refptr<Extension> LoadExtension(const FilePath& extension_root,
                                       Extension::Location location,
                                       int flags,
                                       std::string* error);

bool ValidateExtension(Extension* extension, std::string* error);

void GarbageCollectExtensions(
    const FilePath& extensions_dir,
    const std::map<std::string, FilePath>& extension_paths);

ExtensionMessageBundle* LoadExtensionMessageBundle(
    const FilePath& extension_path,
    const std::string& default_locale,
    std::string* error);

bool CheckForIllegalFilenames(const FilePath& extension_path,
                              std::string* error);

FilePath ExtensionURLToRelativeFilePath(const GURL& url);

FilePath GetUserDataTempDir();

void DeleteFile(const FilePath& path, bool recursive);

}  

#endif  
