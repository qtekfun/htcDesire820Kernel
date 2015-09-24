// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_MANIFEST_URL_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_MANIFEST_URL_HANDLER_H_

#include <string>

#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"

namespace base {
class DictionaryValue;
}

namespace extensions {

struct ManifestURL : public Extension::ManifestData {
  GURL url_;

  
  static const GURL& GetDevToolsPage(const Extension* extension);

  
  
  
  
  static const GURL GetHomepageURL(const Extension* extension);

  
  static const GURL& GetUpdateURL(const Extension* extension);

  
  static bool UpdatesFromGallery(const Extension* extension);
  static bool UpdatesFromGallery(const base::DictionaryValue* manifest);

  
  static const GURL& GetOptionsPage(const Extension* extension);

  
  static const GURL GetDetailsURL(const Extension* extension);
};

struct URLOverrides : public Extension::ManifestData {
  typedef std::map<const std::string, GURL> URLOverrideMap;

  
  URLOverrides();
  virtual ~URLOverrides();

  static const URLOverrideMap&
      GetChromeURLOverrides(const Extension* extension);

  
  
  URLOverrideMap chrome_url_overrides_;
};

class DevToolsPageHandler : public ManifestHandler {
 public:
  DevToolsPageHandler();
  virtual ~DevToolsPageHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(DevToolsPageHandler);
};

class HomepageURLHandler : public ManifestHandler {
 public:
  HomepageURLHandler();
  virtual ~HomepageURLHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(HomepageURLHandler);
};

class UpdateURLHandler : public ManifestHandler {
 public:
  UpdateURLHandler();
  virtual ~UpdateURLHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(UpdateURLHandler);
};

class OptionsPageHandler : public ManifestHandler {
 public:
  OptionsPageHandler();
  virtual ~OptionsPageHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;
  virtual bool Validate(const Extension* extension,
                        std::string* error,
                        std::vector<InstallWarning>* warnings) const OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(OptionsPageHandler);
};

class URLOverridesHandler : public ManifestHandler {
 public:
  URLOverridesHandler();
  virtual ~URLOverridesHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(URLOverridesHandler);
};

}  

#endif  
