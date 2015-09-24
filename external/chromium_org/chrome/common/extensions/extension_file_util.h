// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_FILE_UTIL_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_FILE_UTIL_H_

#include <map>
#include <string>

#include "base/memory/ref_counted.h"
#include "chrome/common/extensions/message_bundle.h"
#include "extensions/common/manifest.h"

class ExtensionIconSet;

namespace base {
class DictionaryValue;
class FilePath;
}

namespace extensions {
class Extension;
class MessageBundle;
struct InstallWarning;
}

namespace extension_file_util {

base::FilePath InstallExtension(const base::FilePath& unpacked_source_dir,
                                const std::string& id,
                                const std::string& version,
                                const base::FilePath& extensions_dir);

void UninstallExtension(const base::FilePath& extensions_dir,
                        const std::string& id);

scoped_refptr<extensions::Extension> LoadExtension(
    const base::FilePath& extension_root,
    extensions::Manifest::Location location,
    int flags,
    std::string* error);

scoped_refptr<extensions::Extension> LoadExtension(
    const base::FilePath& extension_root,
    const std::string& extension_id,
    extensions::Manifest::Location location,
    int flags,
    std::string* error);

base::DictionaryValue* LoadManifest(const base::FilePath& extension_root,
                                    std::string* error);

bool ValidateFilePath(const base::FilePath& path);

bool ValidateExtensionIconSet(const ExtensionIconSet& icon_set,
                              const extensions::Extension* extension,
                              int error_message_id,
                              std::string* error);

bool ValidateExtension(const extensions::Extension* extension,
                       std::string* error,
                       std::vector<extensions::InstallWarning>* warnings);

std::set<base::FilePath> GetBrowserImagePaths(
    const extensions::Extension* extension);

std::vector<base::FilePath> FindPrivateKeyFiles(
    const base::FilePath& extension_dir);

void GarbageCollectExtensions(
    const base::FilePath& extensions_dir,
    const std::multimap<std::string, base::FilePath>& extension_paths);

extensions::MessageBundle* LoadMessageBundle(
    const base::FilePath& extension_path,
    const std::string& default_locale,
    std::string* error);

extensions::MessageBundle::SubstitutionMap* LoadMessageBundleSubstitutionMap(
    const base::FilePath& extension_path,
    const std::string& extension_id,
    const std::string& default_locale);

bool CheckForIllegalFilenames(const base::FilePath& extension_path,
                              std::string* error);

base::FilePath GetInstallTempDir(const base::FilePath& extensions_dir);

void DeleteFile(const base::FilePath& path, bool recursive);

}  

#endif  
