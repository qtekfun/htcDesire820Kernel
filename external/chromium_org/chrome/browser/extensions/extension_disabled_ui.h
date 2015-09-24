// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_DISABLED_UI_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_DISABLED_UI_H_

class Browser;
class ExtensionService;
namespace content {
class WebContents;
}

namespace extensions {

class Extension;

void AddExtensionDisabledError(ExtensionService* service,
                               const Extension* extension);

void ShowExtensionDisabledDialog(ExtensionService* service,
                                 content::WebContents* web_contents,
                                 const Extension* extension);

}  

#endif  
