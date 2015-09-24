// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_FILEICON_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_FILEICON_SOURCE_H_
#pragma once

#include <string>

#include "chrome/browser/icon_manager.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"

class GURL;
class RefCountedBytes;

namespace gfx {
class Image;
}

class FileIconSource : public ChromeURLDataManager::DataSource {
 public:
  explicit FileIconSource();

  
  
  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id);

  virtual std::string GetMimeType(const std::string&) const;

  
  void OnFileIconDataAvailable(
      IconManager::Handle request_handle,
      gfx::Image* icon);

 private:
  virtual ~FileIconSource();

  CancelableRequestConsumerT<int, 0> cancelable_consumer_;

  
  
  scoped_refptr<RefCountedBytes> default_favicon_;

  DISALLOW_COPY_AND_ASSIGN(FileIconSource);
};
#endif  
