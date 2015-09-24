// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_PROFILE_CHOOSER_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_PROFILE_CHOOSER_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/base_bubble_controller.h"

class AvatarMenu;
class Browser;

@interface ProfileChooserController : BaseBubbleController {
 @private
  
  scoped_ptr<AvatarMenu> avatarMenu_;

  
  Browser* browser_;
}

- (id)initWithBrowser:(Browser*)browser anchoredAt:(NSPoint)point;

- (IBAction)addNewProfile:(id)sender;

- (IBAction)switchToProfile:(id)sender;

- (IBAction)showUserManager:(id)sender;

- (IBAction)switchToGuestProfile:(id)sender;

- (IBAction)exitGuestProfile:(id)sender;

- (IBAction)showAccountManagement:(id)sender;

- (IBAction)lockProfile:(id)sender;

- (IBAction)showSigninPage:(id)sender;

@end


@interface ProfileChooserController (ExposedForTesting)
- (id)initWithBrowser:(Browser*)browser anchoredAt:(NSPoint)point;
@end

#endif  
