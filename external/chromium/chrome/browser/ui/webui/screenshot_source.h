// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SCREENSHOT_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_SCREENSHOT_SOURCE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"

class ScreenshotSource : public ChromeURLDataManager::DataSource {
 public:
  explicit ScreenshotSource(
      std::vector<unsigned char>* current_screenshot);

  
  
  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id);

  virtual std::string GetMimeType(const std::string&) const;

  std::vector<unsigned char> GetScreenshot(const std::string& path);

 private:
  virtual ~ScreenshotSource();

  std::vector<unsigned char> current_screenshot_;

  DISALLOW_COPY_AND_ASSIGN(ScreenshotSource);
};

#endif  
