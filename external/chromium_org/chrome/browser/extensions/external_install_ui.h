// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTERNAL_INSTALL_UI_H_
#define CHROME_BROWSER_EXTENSIONS_EXTERNAL_INSTALL_UI_H_

class Browser;
class ExtensionService;

namespace extensions {

class Extension;

void AddExternalInstallError(ExtensionService* service,
                             const Extension* extension,
                             bool is_new_profile);
void RemoveExternalInstallError(ExtensionService* service);

bool HasExternalInstallError(ExtensionService* service);
bool HasExternalInstallBubble(ExtensionService* service);

}  

#endif  
