// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_FAVICON_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_FAVICON_SOURCE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/favicon_service.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"

class GURL;
class Profile;

class FaviconSource : public ChromeURLDataManager::DataSource {
 public:
  explicit FaviconSource(Profile* profile);

  
  
  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id);

  virtual std::string GetMimeType(const std::string&) const;

  virtual bool ShouldReplaceExistingSource() const;

 private:
  
  void OnFaviconDataAvailable(FaviconService::Handle request_handle,
                              history::FaviconData favicon);

  
  void SendDefaultResponse(int request_id);

  virtual ~FaviconSource();

  Profile* profile_;
  CancelableRequestConsumerT<int, 0> cancelable_consumer_;

  
  
  scoped_refptr<RefCountedMemory> default_favicon_;

  DISALLOW_COPY_AND_ASSIGN(FaviconSource);
};

#endif  
