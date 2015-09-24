// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_MIME_TYPES_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_MIME_TYPES_HANDLER_H_

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"
#include "extensions/common/url_pattern.h"
#include "extensions/common/url_pattern_set.h"
#include "url/gurl.h"

class URLPattern;

class MimeTypesHandler {
 public:
  
  static std::vector<std::string> GetMIMETypeWhitelist();

  static MimeTypesHandler* GetHandler(const extensions::Extension* extension);

  MimeTypesHandler();
  ~MimeTypesHandler();

  
  std::string extension_id() const { return extension_id_; }
  void set_extension_id(const std::string& extension_id) {
    extension_id_ = extension_id;
  }

  
  void AddMIMEType(const std::string& mime_type);
  
  bool CanHandleMIMEType(const std::string& mime_type) const;

 private:
  
  
  std::string extension_id_;

  
  std::set<std::string> mime_type_set_;
};

class MimeTypesHandlerParser : public extensions::ManifestHandler {
 public:
  MimeTypesHandlerParser();
  virtual ~MimeTypesHandlerParser();

  virtual bool Parse(extensions::Extension* extension,
                     base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;
};

#endif  

