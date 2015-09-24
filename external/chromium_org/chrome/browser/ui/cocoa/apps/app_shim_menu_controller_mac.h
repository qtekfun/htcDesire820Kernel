// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPS_APP_SHIM_MENU_CONTROLLER_MAC_H_
#define CHROME_BROWSER_UI_COCOA_APPS_APP_SHIM_MENU_CONTROLLER_MAC_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

@class DoppelgangerMenuItem;

@interface AppShimMenuController : NSObject {
 @private
  
  base::scoped_nsobject<NSString> appId_;
  
  base::scoped_nsobject<DoppelgangerMenuItem> aboutDoppelganger_;
  base::scoped_nsobject<DoppelgangerMenuItem> hideDoppelganger_;
  base::scoped_nsobject<DoppelgangerMenuItem> quitDoppelganger_;
  base::scoped_nsobject<DoppelgangerMenuItem> newDoppelganger_;
  base::scoped_nsobject<DoppelgangerMenuItem> openDoppelganger_;
  base::scoped_nsobject<DoppelgangerMenuItem> allToFrontDoppelganger_;
  
  base::scoped_nsobject<NSMenuItem> appMenuItem_;
  base::scoped_nsobject<NSMenuItem> fileMenuItem_;
  base::scoped_nsobject<NSMenuItem> editMenuItem_;
  base::scoped_nsobject<NSMenuItem> windowMenuItem_;
}

@end

#endif  
