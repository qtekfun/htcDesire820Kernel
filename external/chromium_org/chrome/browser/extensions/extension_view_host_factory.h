// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_VIEW_HOST_FACTORY_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_VIEW_HOST_FACTORY_H_

#include "base/basictypes.h"

class Browser;
class GURL;
class Profile;

namespace extensions {

class ExtensionViewHost;

class ExtensionViewHostFactory {
 public:
  
  
  
  static ExtensionViewHost* CreatePopupHost(const GURL& url, Browser* browser);
  static ExtensionViewHost* CreateInfobarHost(const GURL& url,
                                              Browser* browser);

  
  
  static ExtensionViewHost* CreateDialogHost(const GURL& url, Profile* profile);

 private:
  DISALLOW_COPY_AND_ASSIGN(ExtensionViewHostFactory);
};

}  

#endif  
