// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MAC_DOCK_H_
#define CHROME_BROWSER_MAC_DOCK_H_

#if defined(__OBJC__)
@class NSString;
#else
class NSString;
#endif

namespace dock {

enum AddIconStatus {
  IconAddFailure,
  IconAddSuccess,
  IconAlreadyPresent
};

AddIconStatus AddIcon(NSString* installed_path, NSString* dmg_app_path);

}  

#endif  
