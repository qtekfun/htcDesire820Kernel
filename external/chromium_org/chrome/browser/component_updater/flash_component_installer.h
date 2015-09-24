// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_FLASH_COMPONENT_INSTALLER_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_FLASH_COMPONENT_INSTALLER_H_

class ComponentUpdateService;

namespace base {
class DictionaryValue;
class Version;
}

void RegisterPepperFlashComponent(ComponentUpdateService* cus);

bool CheckPepperFlashManifest(const base::DictionaryValue& manifest,
                              base::Version* version_out);

#endif  
