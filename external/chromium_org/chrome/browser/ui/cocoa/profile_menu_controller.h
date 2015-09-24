// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_PROFILE_MENU_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_PROFILE_MENU_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"

class AvatarMenu;
class Browser;

namespace ProfileMenuControllerInternal {
class Observer;
}

@interface ProfileMenuController : NSObject {
 @private
  
  scoped_ptr<AvatarMenu> menu_;

  
  
  scoped_ptr<ProfileMenuControllerInternal::Observer> observer_;

  
  __weak NSMenuItem* mainMenuItem_;
}

- (id)initWithMainMenuItem:(NSMenuItem*)item;

- (IBAction)switchToProfileFromMenu:(id)sender;
- (IBAction)switchToProfileFromDock:(id)sender;
- (IBAction)editProfile:(id)sender;
- (IBAction)newProfile:(id)sender;

- (BOOL)insertItemsIntoMenu:(NSMenu*)menu
                   atOffset:(NSInteger)offset
                   fromDock:(BOOL)dock;

@end

@interface ProfileMenuController (PrivateExposedForTesting)
- (NSMenu*)menu;
- (void)rebuildMenu;
- (NSMenuItem*)createItemWithTitle:(NSString*)title action:(SEL)sel;
- (void)activeBrowserChangedTo:(Browser*)browser;
@end

#endif  
