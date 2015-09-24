// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_UBER_UBER_UI_H_
#define CHROME_BROWSER_UI_WEBUI_UBER_UBER_UI_H_

#include <string>

#include "base/memory/scoped_vector.h"
#include "base/values.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui_controller.h"

class UberUI : public content::WebUIController {
 public:
  explicit UberUI(content::WebUI* web_ui);
  virtual ~UberUI();

  
  virtual bool OverrideHandleWebUIMessage(const GURL& source_url,
                                          const std::string& message,
                                          const ListValue& args) OVERRIDE;

  
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void RenderViewReused(
      content::RenderViewHost* render_view_host) OVERRIDE;

 private:
  
  typedef std::map<std::string, content::WebUI*> SubpageMap;

  
  void RegisterSubpage(const std::string& page_url);

  
  SubpageMap sub_uis_;

  DISALLOW_COPY_AND_ASSIGN(UberUI);
};

class UberFrameUI : public content::NotificationObserver,
                    public content::WebUIController {
 public:
  explicit UberFrameUI(content::WebUI* web_ui);
  virtual ~UberFrameUI();

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(UberFrameUI);
};

#endif  
