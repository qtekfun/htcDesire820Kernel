// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_DISABLED_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_DISABLED_INFOBAR_DELEGATE_H_
#pragma once

class Extension;
class ExtensionService;
class Profile;

void ShowExtensionDisabledUI(ExtensionService* service, Profile* profile,
                             const Extension* extension);

void ShowExtensionDisabledDialog(ExtensionService* service, Profile* profile,
                                 const Extension* extension);

#endif  
