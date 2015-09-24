// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_I18N_DEFAULT_LOCALE_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_API_I18N_DEFAULT_LOCALE_HANDLER_H_

#include <string>

#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"

namespace extensions {

struct LocaleInfo : public Extension::ManifestData {
  
  std::string default_locale;

  static const std::string& GetDefaultLocale(const Extension* extension);
};

class DefaultLocaleHandler : public ManifestHandler {
 public:
  DefaultLocaleHandler();
  virtual ~DefaultLocaleHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

  
  virtual bool Validate(const Extension* extension,
                        std::string* error,
                        std::vector<InstallWarning>* warnings) const OVERRIDE;

  virtual bool AlwaysValidateForType(Manifest::Type type) const OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(DefaultLocaleHandler);
};

}  

#endif  
