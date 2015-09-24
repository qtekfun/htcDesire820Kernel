// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/base_bubble_controller.h"

class Browser;
class Profile;

@interface FirstRunBubbleController : BaseBubbleController {
 @private
  IBOutlet NSTextField* header_;
  Browser* browser_;
  Profile* profile_;
}

+ (FirstRunBubbleController*) showForView:(NSView*)view
                                   offset:(NSPoint)offset
                                  browser:(Browser*)browser
                                  profile:(Profile*)profile;

- (IBAction)onChange:(id)sender;

@end
