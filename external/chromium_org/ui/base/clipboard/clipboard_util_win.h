// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_CLIPBOARD_CLIPBOARD_UTIL_WIN_H_
#define UI_BASE_CLIPBOARD_CLIPBOARD_UTIL_WIN_H_

#include <shlobj.h>
#include <map>
#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "ui/base/ui_export.h"

namespace ui {

class UI_EXPORT ClipboardUtil {
 public:
  
  
  
  static bool HasUrl(IDataObject* data_object);
  static bool HasFilenames(IDataObject* data_object);
  static bool HasPlainText(IDataObject* data_object);
  static bool HasFileContents(IDataObject* data_object);
  static bool HasHtml(IDataObject* data_object);

  
  
  
  static bool GetUrl(IDataObject* data_object,
                     string16* url,
                     string16* title,
                     bool convert_filenames);
  static bool GetFilenames(IDataObject* data_object,
                           std::vector<string16>* filenames);
  static bool GetPlainText(IDataObject* data_object, string16* plain_text);
  static bool GetHtml(IDataObject* data_object,
                      string16* text_html,
                      std::string* base_url);
  static bool GetFileContents(IDataObject* data_object,
                              string16* filename,
                              std::string* file_contents);
  
  
  
  
  static bool GetWebCustomData(IDataObject* data_object,
                               std::map<string16, string16>* custom_data);

  
  
  static std::string HtmlToCFHtml(const std::string& html,
                                  const std::string& base_url);
  static void CFHtmlToHtml(const std::string& cf_html,
                           std::string* html,
                           std::string* base_url);
  static void CFHtmlExtractMetadata(const std::string& cf_html,
                                    std::string* base_url,
                                    size_t* html_start,
                                    size_t* fragment_start,
                                    size_t* fragment_end);
};

}

#endif  
