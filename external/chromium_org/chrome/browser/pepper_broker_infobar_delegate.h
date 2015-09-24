// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PEPPER_BROKER_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_PEPPER_BROKER_INFOBAR_DELEGATE_H_

#include "base/callback.h"
#include "base/files/file_path.h"
#include "chrome/browser/infobars/confirm_infobar_delegate.h"
#include "url/gurl.h"

class HostContentSettingsMap;
class InfoBarService;
class TabSpecificContentSettings;

namespace content {
class WebContents;
}

class PepperBrokerInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  
  
  
  
  static void Create(content::WebContents* web_contents,
                     const GURL& url,
                     const base::FilePath& plugin_path,
                     const base::Callback<void(bool)>& callback);

 private:
  PepperBrokerInfoBarDelegate(const GURL& url,
                              const base::FilePath& plugin_path,
                              const std::string& languages,
                              HostContentSettingsMap* content_settings,
                              TabSpecificContentSettings* tab_content_settings,
                              const base::Callback<void(bool)>& callback);
  virtual ~PepperBrokerInfoBarDelegate();

  
  virtual int GetIconID() const OVERRIDE;
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual base::string16 GetLinkText() const OVERRIDE;
  virtual bool LinkClicked(WindowOpenDisposition disposition) OVERRIDE;

  void DispatchCallback(bool result);

  const GURL url_;
  const base::FilePath plugin_path_;
  const std::string languages_;
  HostContentSettingsMap* content_settings_;
  TabSpecificContentSettings* tab_content_settings_;
  base::Callback<void(bool)> callback_;

  DISALLOW_COPY_AND_ASSIGN(PepperBrokerInfoBarDelegate);
};

#endif  
