// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MAC_INSTALL_FROM_DMG_H_
#define CHROME_BROWSER_MAC_INSTALL_FROM_DMG_H_

#include <string>

bool MaybeInstallFromDiskImage();

void EjectAndTrashDiskImage(const std::string& dmg_bsd_device_name);

#endif  
