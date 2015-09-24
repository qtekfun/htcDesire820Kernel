// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_THEME_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_THEME_HANDLER_H_

#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"

namespace base {
class DictionaryValue;
}

namespace extensions {

struct ThemeInfo : public Extension::ManifestData {
  
  ThemeInfo();
  virtual ~ThemeInfo();

  static const base::DictionaryValue* GetImages(const Extension* extension);
  static const base::DictionaryValue* GetColors(const Extension* extension);
  static const base::DictionaryValue* GetTints(const Extension* extension);
  static const base::DictionaryValue* GetDisplayProperties(
      const Extension* extension);

  
  scoped_ptr<base::DictionaryValue> theme_images_;

  
  scoped_ptr<base::DictionaryValue> theme_colors_;

  
  scoped_ptr<base::DictionaryValue> theme_tints_;

  
  scoped_ptr<base::DictionaryValue> theme_display_properties_;
};

class ThemeHandler : public ManifestHandler {
 public:
  ThemeHandler();
  virtual ~ThemeHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;
  virtual bool Validate(const Extension* extension,
                        std::string* error,
                        std::vector<InstallWarning>* warnings) const OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ThemeHandler);
};

}  

#endif  
