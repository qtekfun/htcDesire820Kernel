// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WEB_UI_CONTROLLER_FACTORY_H_
#define CONTENT_PUBLIC_BROWSER_WEB_UI_CONTROLLER_FACTORY_H_

#include "content/common/content_export.h"
#include "content/public/browser/web_ui.h"

class GURL;

namespace content {

class BrowserContext;
class WebContents;
class WebUIController;

class CONTENT_EXPORT WebUIControllerFactory {
 public:
  virtual ~WebUIControllerFactory() {}

  
  static void RegisterFactory(WebUIControllerFactory* factory);

  static void UnregisterFactoryForTesting(WebUIControllerFactory* factory);

  
  
  virtual WebUIController* CreateWebUIControllerForURL(
      WebUI* web_ui, const GURL& url) const = 0;

  
  
  
  virtual WebUI::TypeID GetWebUIType(BrowserContext* browser_context,
                                     const GURL& url) const = 0;

  
  
  
  virtual bool UseWebUIForURL(BrowserContext* browser_context,
                              const GURL& url) const = 0;

  
  virtual bool UseWebUIBindingsForURL(BrowserContext* browser_context,
                                      const GURL& url) const = 0;
};

}  

#endif  
