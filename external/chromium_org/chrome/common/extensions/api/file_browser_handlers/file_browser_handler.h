// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_FILE_BROWSER_HANDLERS_FILE_BROWSER_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_API_FILE_BROWSER_HANDLERS_FILE_BROWSER_HANDLER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"
#include "extensions/common/url_pattern.h"
#include "extensions/common/url_pattern_set.h"

class GURL;
class URLPattern;

class FileBrowserHandler {
 public:
  typedef std::vector<linked_ptr<FileBrowserHandler> > List;

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

  
  const extensions::URLPatternSet& file_url_patterns() const {
    return url_set_;
  }
  void AddPattern(const URLPattern& pattern);
  bool MatchesURL(const GURL& url) const;
  void ClearPatterns();

  
  const std::string icon_path() const { return default_icon_path_; }
  void set_icon_path(const std::string& path) {
    default_icon_path_ = path;
  }

  
  
  bool AddFileAccessPermission(const std::string& permission_str);
  
  
  
  
  bool ValidateFileAccessPermissions();
  
  bool CanRead() const;
  
  bool CanWrite() const;
  
  bool HasCreateAccessPermission() const;

  
  static List* GetHandlers(const extensions::Extension* extension);

 private:
  
  
  std::string extension_id_;
  std::string title_;
  std::string default_icon_path_;
  
  std::string id_;
  unsigned int file_access_permission_flags_;

  
  extensions::URLPatternSet url_set_;
};

class FileBrowserHandlerParser : public extensions::ManifestHandler {
 public:
  FileBrowserHandlerParser();
  virtual ~FileBrowserHandlerParser();

  virtual bool Parse(extensions::Extension* extension,
                     base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;
};

#endif  
