// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_USER_IMAGE_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CHROMEOS_USER_IMAGE_SOURCE_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"

namespace chromeos {

class UserImageSource : public ChromeURLDataManager::DataSource {
 public:
  UserImageSource();

  
  
  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id);

  virtual std::string GetMimeType(const std::string&) const;

  
  
  std::vector<unsigned char> GetUserImage(const std::string& email) const;

 private:
  virtual ~UserImageSource();

  DISALLOW_COPY_AND_ASSIGN(UserImageSource);
};

}  

#endif  
