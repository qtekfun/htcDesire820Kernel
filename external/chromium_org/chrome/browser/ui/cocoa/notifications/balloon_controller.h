// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_NOTIFICATIONS_BALLOON_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_NOTIFICATIONS_BALLOON_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"

class Balloon;
@class BalloonContentViewCocoa;
@class BalloonShelfViewCocoa;
class BalloonViewHost;
@class HoverImageButton;
@class MenuController;
class NotificationOptionsMenuModel;

@interface BalloonController : NSWindowController<NSWindowDelegate> {
 @private
  
  
  Balloon* balloon_;

  
  IBOutlet BalloonContentViewCocoa* htmlContainer_;

  
  IBOutlet BalloonShelfViewCocoa* shelf_;

  
  IBOutlet NSButton* closeButton_;

  
  int closeButtonTrackingTag_;

  
  IBOutlet NSTextField* originLabel_;

  
  IBOutlet HoverImageButton* optionsButton_;
  scoped_ptr<NotificationOptionsMenuModel> menuModel_;
  base::scoped_nsobject<MenuController> menuController_;

  
  scoped_ptr<BalloonViewHost> htmlContents_;

  
  BOOL optionMenuIsActive_;
  BOOL delayedClose_;
}

- (id)initWithBalloon:(Balloon*)balloon;

- (IBAction)closeButtonPressed:(id)sender;

- (IBAction)optionsButtonPressed:(id)sender;

- (IBAction)permissionRevoked:(id)sender;

- (void)closeBalloon:(bool)byUser;

- (void)updateContents;

- (void)repositionToBalloon;

- (int)desiredTotalWidth;
- (int)desiredTotalHeight;

- (BalloonViewHost*)getHost;
@end

@interface BalloonController (UnitTesting)
- (void)initializeHost;
@end

#endif  
