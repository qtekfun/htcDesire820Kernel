// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_SPELLCHECK_SPELLCHECK_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_API_SPELLCHECK_SPELLCHECK_HANDLER_H_

#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"

namespace extensions {

struct SpellcheckDictionaryInfo : public extensions::Extension::ManifestData {
  SpellcheckDictionaryInfo();
  virtual ~SpellcheckDictionaryInfo();

  std::string language;
  std::string locale;
  std::string path;
  std::string format;
};

class SpellcheckHandler : public ManifestHandler {
 public:
  SpellcheckHandler();
  virtual ~SpellcheckHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(SpellcheckHandler);
};

}  

#endif  
