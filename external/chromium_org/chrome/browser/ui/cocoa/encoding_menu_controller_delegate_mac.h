// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_ENCODING_MENU_CONTROLLER_DELEGATE_MAC_H_
#define CHROME_BROWSER_UI_COCOA_ENCODING_MENU_CONTROLLER_DELEGATE_MAC_H_

#include "base/basictypes.h"  

@class NSMenu;
class Profile;

class EncodingMenuControllerDelegate {
 public:
  static void BuildEncodingMenu(Profile *profile, NSMenu* encoding_menu);
 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(EncodingMenuControllerDelegate);
};

#endif  
