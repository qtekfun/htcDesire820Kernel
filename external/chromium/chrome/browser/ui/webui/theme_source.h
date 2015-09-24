// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_THEME_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_THEME_SOURCE_H_
#pragma once

#include <string>

#include "chrome/browser/ui/webui/chrome_url_data_manager.h"

class Profile;
class RefCountedBytes;

class ThemeSource : public ChromeURLDataManager::DataSource {
 public:
  explicit ThemeSource(Profile* profile);

  
  
  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id);
  virtual std::string GetMimeType(const std::string& path) const;

  
  virtual MessageLoop* MessageLoopForRequestPath(const std::string& path) const;

  virtual bool ShouldReplaceExistingSource() const;

 protected:
  virtual ~ThemeSource();

 private:
  
  void SendThemeBitmap(int request_id, int resource_id);

  
  Profile* profile_;

  
  scoped_refptr<RefCountedBytes> css_bytes_;

  DISALLOW_COPY_AND_ASSIGN(ThemeSource);
};

#endif  
