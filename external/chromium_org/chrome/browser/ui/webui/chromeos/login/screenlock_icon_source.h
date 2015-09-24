// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_SCREENLOCK_ICON_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_SCREENLOCK_ICON_SOURCE_H_

#include "base/memory/weak_ptr.h"
#include "content/public/browser/url_data_source.h"

namespace chromeos {

class ScreenlockIconProvider;

class ScreenlockIconSource : public content::URLDataSource {
 public:
  explicit ScreenlockIconSource(
      base::WeakPtr<ScreenlockIconProvider> icon_provider);

  
  virtual std::string GetSource() const OVERRIDE;
  virtual void StartDataRequest(
      const std::string& path,
      int render_process_id,
      int render_view_id,
      const content::URLDataSource::GotDataCallback& callback) OVERRIDE;

  virtual std::string GetMimeType(const std::string& path) const OVERRIDE;

  
  static std::string GetIconURLForUser(const std::string& username);

 private:
  virtual ~ScreenlockIconSource();

  base::WeakPtr<ScreenlockIconProvider> icon_provider_;

  DISALLOW_COPY_AND_ASSIGN(ScreenlockIconSource);
};

}  

#endif  
