// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_BASE_WEBUI_JSTEMPLATE_BUILDER_H_
#define UI_BASE_WEBUI_JSTEMPLATE_BUILDER_H_

#include <string>

#include "base/strings/string_piece.h"
#include "ui/base/ui_export.h"

namespace base {
class DictionaryValue;
}

namespace webui {

class UI_EXPORT UseVersion2 {
 public:
  UseVersion2();
  ~UseVersion2();

 private:
  DISALLOW_COPY_AND_ASSIGN(UseVersion2);
};

UI_EXPORT std::string GetTemplateHtml(const base::StringPiece& html_template,
                                      const base::DictionaryValue* json,
                                      const base::StringPiece& template_id);

UI_EXPORT std::string GetI18nTemplateHtml(
    const base::StringPiece& html_template,
    const base::DictionaryValue* json);

UI_EXPORT std::string GetTemplatesHtml(const base::StringPiece& html_template,
                                       const base::DictionaryValue* json,
                                       const base::StringPiece& template_id);


UI_EXPORT void AppendJsonHtml(const base::DictionaryValue* json,
                              std::string* output);

UI_EXPORT void AppendJsonJS(const base::DictionaryValue* json,
                            std::string* output);

UI_EXPORT void AppendJsTemplateSourceHtml(std::string* output);

UI_EXPORT void AppendJsTemplateProcessHtml(const base::StringPiece& template_id,
                                           std::string* output);

UI_EXPORT void AppendI18nTemplateSourceHtml(std::string* output);

UI_EXPORT void AppendI18nTemplateProcessHtml(std::string* output);

}  

#endif  
