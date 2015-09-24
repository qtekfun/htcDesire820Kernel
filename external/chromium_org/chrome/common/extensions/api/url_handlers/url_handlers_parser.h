// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_URL_HANDLERS_URL_HANDLERS_PARSER_H_
#define CHROME_COMMON_EXTENSIONS_API_URL_HANDLERS_URL_HANDLERS_PARSER_H_

#include <string>
#include <vector>

#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"
#include "extensions/common/url_pattern.h"

class GURL;

namespace extensions {

struct UrlHandlerInfo {
  UrlHandlerInfo();
  ~UrlHandlerInfo();

  
  std::string id;
  
  std::string title;
  
  URLPatternSet patterns;
};

struct UrlHandlers : public Extension::ManifestData {
  UrlHandlers();
  virtual ~UrlHandlers();

  
  static const std::vector<UrlHandlerInfo>* GetUrlHandlers(
      const Extension* extension);

  
  
  static bool CanExtensionHandleUrl(
      const Extension* extension,
      const GURL& url);

  
  
  static const UrlHandlerInfo* FindMatchingUrlHandler(
      const Extension* extension,
      const GURL& url);

  std::vector<UrlHandlerInfo> handlers;
};

class UrlHandlersParser : public ManifestHandler {
 public:
  UrlHandlersParser();
  virtual ~UrlHandlersParser();

  
  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(UrlHandlersParser);
};

}  

#endif  
