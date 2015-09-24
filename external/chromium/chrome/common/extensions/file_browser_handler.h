// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_FILE_BROWSER_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_FILE_BROWSER_HANDLER_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chrome/common/extensions/url_pattern.h"
#include "googleurl/src/gurl.h"

class URLPattern;

class FileBrowserHandler {
 public:
  typedef std::vector<URLPattern> PatternList;

  FileBrowserHandler();
  ~FileBrowserHandler();

  
  std::string extension_id() const { return extension_id_; }
  void set_extension_id(const std::string& extension_id) {
    extension_id_ = extension_id;
  }

  
  const std::string& id() const { return id_; }
  void set_id(const std::string& id) { id_ = id; }

  
  const std::string& title() const { return title_; }
  void set_title(const std::string& title) { title_ = title; }

  
  const PatternList& file_url_patterns() const { return patterns_; }
  void AddPattern(const URLPattern& pattern);
  bool MatchesURL(const GURL& url) const;
  void ClearPatterns();

  
  const std::string icon_path() const { return default_icon_path_; }
  void set_icon_path(const std::string& path) {
    default_icon_path_ = path;
  }

 private:
  
  
  std::string extension_id_;
  std::string title_;
  std::string default_icon_path_;
  
  std::string id_;
  
  PatternList patterns_;
};

#endif  
