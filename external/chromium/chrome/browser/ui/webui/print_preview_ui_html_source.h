// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_PRINT_PREVIEW_UI_HTML_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_PRINT_PREVIEW_UI_HTML_SOURCE_H_
#pragma once

#include <string>
#include <utility>

#include "chrome/browser/ui/webui/chrome_url_data_manager.h"

namespace base {
class SharedMemory;
}

class PrintPreviewUIHTMLSource : public ChromeURLDataManager::DataSource {
 public:
  
  
  typedef std::pair<base::SharedMemory*, uint32> PrintPreviewData;

  PrintPreviewUIHTMLSource();
  virtual ~PrintPreviewUIHTMLSource();

  
  
  void GetPrintPreviewData(PrintPreviewData* data);

  
  
  
  
  void SetPrintPreviewData(const PrintPreviewData& data);

  
  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id);
  virtual std::string GetMimeType(const std::string&) const;

 private:
  
  
  PrintPreviewData data_;

  DISALLOW_COPY_AND_ASSIGN(PrintPreviewUIHTMLSource);
};

#endif  
