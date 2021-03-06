// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_RECOVERY_COMPONENT_INSTALLER_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_RECOVERY_COMPONENT_INSTALLER_H_

class ComponentUpdateService;
class PrefRegistrySimple;
class PrefService;

void RegisterRecoveryComponent(ComponentUpdateService* cus,
                               PrefService* prefs);


void RegisterPrefsForRecoveryComponent(PrefRegistrySimple* registry);

#endif  

