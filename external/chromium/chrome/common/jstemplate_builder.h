// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_COMMON_JSTEMPLATE_BUILDER_H_
#define CHROME_COMMON_JSTEMPLATE_BUILDER_H_
#pragma once

#include <string>

class DictionaryValue;
namespace base {
class StringPiece;
}

namespace jstemplate_builder {

std::string GetTemplateHtml(const base::StringPiece& html_template,
                            const DictionaryValue* json,
                            const base::StringPiece& template_id);

std::string GetI18nTemplateHtml(const base::StringPiece& html_template,
                                const DictionaryValue* json);

std::string GetTemplatesHtml(const base::StringPiece& html_template,
                             const DictionaryValue* json,
                             const base::StringPiece& template_id);


void AppendJsonHtml(const DictionaryValue* json, std::string* output);

void AppendJsTemplateSourceHtml(std::string* output);

void AppendJsTemplateProcessHtml(const base::StringPiece& template_id,
                                 std::string* output);

void AppendI18nTemplateSourceHtml(std::string* output);

void AppendI18nTemplateProcessHtml(std::string* output);

}  
#endif  
