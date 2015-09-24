// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_FILE_HANDLERS_APP_FILE_HANDLER_UTIL_H_
#define CHROME_BROWSER_EXTENSIONS_API_FILE_HANDLERS_APP_FILE_HANDLER_UTIL_H_

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "chrome/common/extensions/api/file_handlers/file_handlers_parser.h"
#include "content/public/browser/render_view_host.h"
#include "extensions/common/extension.h"

class Profile;

namespace extensions {
class ExtensionPrefs;

namespace app_file_handler_util {

extern const char kInvalidParameters[];
extern const char kSecurityError[];

typedef std::set<std::pair<base::FilePath, std::string> > PathAndMimeTypeSet;

const FileHandlerInfo* FileHandlerForId(const Extension& app,
                                        const std::string& handler_id);

const FileHandlerInfo* FirstFileHandlerForFile(
    const Extension& app,
    const std::string& mime_type,
    const base::FilePath& path);

std::vector<const FileHandlerInfo*>
FindFileHandlersForFiles(const Extension& extension,
                         const PathAndMimeTypeSet& files);

bool FileHandlerCanHandleFile(
    const FileHandlerInfo& handler,
    const std::string& mime_type,
    const base::FilePath& path);

struct GrantedFileEntry {
  GrantedFileEntry();

  std::string id;
  std::string filesystem_id;
  std::string registered_name;
};

GrantedFileEntry CreateFileEntry(
    Profile* profile,
    const Extension* extension,
    int renderer_id,
    const base::FilePath& path,
    bool is_directory);

void CheckWritableFiles(
    const std::vector<base::FilePath>& paths,
    Profile* profile,
    bool is_directory,
    const base::Closure& on_success,
    const base::Callback<void(const base::FilePath&)>& on_failure);

bool HasFileSystemWritePermission(const Extension* extension);

bool ValidateFileEntryAndGetPath(
    const std::string& filesystem_name,
    const std::string& filesystem_path,
    const content::RenderViewHost* render_view_host,
    base::FilePath* file_path,
    std::string* error);

}  

}  

#endif  
