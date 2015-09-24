// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APPS_APPS_CLIENT_H_
#define APPS_APPS_CLIENT_H_

#include <vector>

namespace content {
class BrowserContext;
}

namespace extensions {
class Extension;
}

namespace apps {

class AppsClient {
 public:
  
  virtual std::vector<content::BrowserContext*> GetLoadedBrowserContexts() = 0;

  
  
  virtual bool CheckAppLaunch(content::BrowserContext* context,
                              const extensions::Extension* extension) = 0;

  
  static AppsClient* Get();

  
  static void Set(AppsClient* client);
};

}  

#endif  
