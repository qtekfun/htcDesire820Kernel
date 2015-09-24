// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROME_WEB_UI_CONTROLLER_FACTORY_H_
#define CHROME_BROWSER_UI_WEBUI_CHROME_WEB_UI_CONTROLLER_FACTORY_H_

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_controller_factory.h"
#include "chrome/browser/favicon/favicon_service.h"

class Profile;

namespace base {
class RefCountedMemory;
}

class ChromeWebUIControllerFactory : public content::WebUIControllerFactory {
 public:
  virtual content::WebUI::TypeID GetWebUIType(
      content::BrowserContext* browser_context,
      const GURL& url) const OVERRIDE;
  virtual bool UseWebUIForURL(content::BrowserContext* browser_context,
                              const GURL& url) const OVERRIDE;
  virtual bool UseWebUIBindingsForURL(content::BrowserContext* browser_context,
                                      const GURL& url) const OVERRIDE;
  virtual content::WebUIController* CreateWebUIControllerForURL(
      content::WebUI* web_ui,
      const GURL& url) const OVERRIDE;

  
  
  void GetFaviconForURL(
      Profile* profile,
      const GURL& page_url,
      const std::vector<ui::ScaleFactor>& scale_factors,
      const FaviconService::FaviconResultsCallback& callback) const;

  static ChromeWebUIControllerFactory* GetInstance();

 protected:
  ChromeWebUIControllerFactory();
  virtual ~ChromeWebUIControllerFactory();

 private:
  friend struct DefaultSingletonTraits<ChromeWebUIControllerFactory>;

  
  
  
  
  
  base::RefCountedMemory* GetFaviconResourceBytes(
      const GURL& page_url, ui::ScaleFactor scale_factor) const;

  DISALLOW_COPY_AND_ASSIGN(ChromeWebUIControllerFactory);
};

#endif  
