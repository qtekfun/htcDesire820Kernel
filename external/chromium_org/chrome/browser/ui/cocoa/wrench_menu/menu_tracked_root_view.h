// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_WRENCH_MENU_MENU_TRACKED_ROOT_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_WRENCH_MENU_MENU_TRACKED_ROOT_VIEW_H_

#import <Cocoa/Cocoa.h>

@interface MenuTrackedRootView : NSView {
 @private
  
  NSMenuItem* menuItem_;  
}

@property(assign, nonatomic) NSMenuItem* menuItem;

@end

#endif  
