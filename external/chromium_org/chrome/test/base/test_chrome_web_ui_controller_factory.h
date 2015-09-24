// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_TEST_CHROME_WEB_UI_CONTROLLER_FACTORY_H_
#define CHROME_TEST_BASE_TEST_CHROME_WEB_UI_CONTROLLER_FACTORY_H_

#include <functional>
#include <map>
#include <string>

#include "chrome/browser/ui/webui/chrome_web_ui_controller_factory.h"
#include "content/public/browser/web_ui.h"

class TestChromeWebUIControllerFactory : public ChromeWebUIControllerFactory {
 public:
  
  class WebUIProvider {
   public:
    
    
     virtual content::WebUIController* NewWebUI(content::WebUI* web_ui,
                                                const GURL& url) = 0;

   protected:
    virtual ~WebUIProvider();
  };

  typedef std::map<std::string, WebUIProvider*> FactoryOverridesMap;

  TestChromeWebUIControllerFactory();
  virtual ~TestChromeWebUIControllerFactory();

  
  void AddFactoryOverride(const std::string& host, WebUIProvider* provider);

  
  void RemoveFactoryOverride(const std::string& host);

  
  virtual content::WebUI::TypeID GetWebUIType(
      content::BrowserContext* browser_context,
      const GURL& url) const OVERRIDE;
  virtual content::WebUIController* CreateWebUIControllerForURL(
      content::WebUI* web_ui, const GURL& url) const OVERRIDE;

 private:
  
  WebUIProvider* GetWebUIProvider(Profile* profile, const GURL& url) const;

  
  FactoryOverridesMap factory_overrides_;

  DISALLOW_COPY_AND_ASSIGN(TestChromeWebUIControllerFactory);
};

#endif  
