// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_AVATAR_MENU_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_AVATAR_MENU_BUBBLE_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/base_bubble_controller.h"
#import "ui/base/cocoa/tracking_area.h"

class AvatarMenu;
class Browser;

@interface AvatarMenuBubbleController : BaseBubbleController {
 @private
  
  scoped_ptr<AvatarMenu> menu_;

  
  base::scoped_nsobject<NSMutableArray> items_;

  
  BOOL expanded_;
}

- (id)initWithBrowser:(Browser*)parentBrowser
           anchoredAt:(NSPoint)point;

- (IBAction)newProfile:(id)sender;

- (IBAction)switchToProfile:(id)sender;

- (IBAction)editProfile:(id)sender;

- (IBAction)switchProfile:(id)sender;

@end


@interface AvatarMenuItemController : NSViewController<NSAnimationDelegate> {
 @private
  
  __weak AvatarMenuBubbleController* controller_;

  
  size_t menuIndex_;

  
  BOOL isHighlighted_;

  
  
  base::scoped_nsobject<NSAnimation> linkAnimation_;

  
  __weak NSImageView* iconView_;
  __weak NSImageView* activeView_;
  __weak NSTextField* nameField_;
  
  
  
  __weak NSTextField* emailField_;
  __weak NSButton* editButton_;
}
@property(readonly, nonatomic) size_t menuIndex;
@property(assign, nonatomic) BOOL isHighlighted;
@property(assign, nonatomic) IBOutlet NSImageView* iconView;
@property(assign, nonatomic) IBOutlet NSImageView* activeView;
@property(assign, nonatomic) IBOutlet NSTextField* nameField;
@property(assign, nonatomic) IBOutlet NSTextField* emailField;
@property(assign, nonatomic) IBOutlet NSButton* editButton;

- (id)initWithMenuIndex:(size_t)menuIndex
          menuController:(AvatarMenuBubbleController*)controller;

- (IBAction)switchToProfile:(id)sender;
- (IBAction)editProfile:(id)sender;

- (void)highlightForEventType:(NSEventType)type;

@end


@interface AvatarMenuItemView : NSView {
 @private
  
  __weak AvatarMenuItemController* viewController_;

  
  ui::ScopedCrTrackingArea trackingArea_;
}
@property(assign, nonatomic) IBOutlet AvatarMenuItemController* viewController;
@end


@interface AccessibilityIgnoredImageCell : NSImageCell
@end

@interface AccessibilityIgnoredTextFieldCell : NSTextFieldCell
@end


@interface AvatarMenuBubbleController (ExposedForTesting)
- (id)initWithMenu:(AvatarMenu*)menu
       parentWindow:(NSWindow*)parent
         anchoredAt:(NSPoint)point;
- (void)performLayout;
- (NSMutableArray*)items;
@end

@interface AvatarMenuItemController (ExposedForTesting)
- (void)willStartAnimation:(NSAnimation*)animation;
@end

#endif  
