// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_IMPORTER_BOOKMARK_HTML_READER_H_
#define CHROME_UTILITY_IMPORTER_BOOKMARK_HTML_READER_H_

#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/strings/string16.h"

class GURL;
struct ImportedBookmarkEntry;
struct ImportedFaviconUsage;

namespace base {
class FilePath;
class Time;
}

namespace bookmark_html_reader {

void ImportBookmarksFile(
    const base::Callback<bool(void)>& cancellation_callback,
    const base::Callback<bool(const GURL&)>& valid_url_callback,
    const base::FilePath& file_path,
    std::vector<ImportedBookmarkEntry>* bookmarks,
    std::vector<ImportedFaviconUsage>* favicons);

namespace internal {


bool ParseCharsetFromLine(const std::string& line,
                          std::string* charset);
bool ParseFolderNameFromLine(const std::string& line,
                             const std::string& charset,
                             base::string16* folder_name,
                             bool* is_toolbar_folder,
                             base::Time* add_date);
bool ParseBookmarkFromLine(const std::string& line,
                           const std::string& charset,
                           base::string16* title,
                           GURL* url,
                           GURL* favicon,
                           base::string16* shortcut,
                           base::Time* add_date,
                           base::string16* post_data);
bool ParseMinimumBookmarkFromLine(const std::string& line,
                                  const std::string& charset,
                                  base::string16* title,
                                  GURL* url);

}  

}  

#endif  
