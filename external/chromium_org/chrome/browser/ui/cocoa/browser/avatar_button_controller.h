// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_AVATAR_BUTTON_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_AVATAR_BUTTON_CONTROLLER_H_

#import <AppKit/AppKit.h>

#import "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"

@class AvatarLabelButton;
@class BaseBubbleController;
class Browser;

namespace AvatarButtonControllerInternal {
class Observer;
}

@interface AvatarButtonController : NSViewController {
 @private
  Browser* browser_;

  
  scoped_ptr<AvatarButtonControllerInternal::Observer> observer_;

  
  __weak BaseBubbleController* menuController_;

  
  base::scoped_nsobject<NSButton> button_;

  
  base::scoped_nsobject<AvatarLabelButton> labelButton_;
}

@property(readonly, nonatomic) NSButton* buttonView;

@property(readonly, nonatomic) NSButton* labelButtonView;

- (id)initWithBrowser:(Browser*)browser;

- (void)setImage:(NSImage*)image;

- (void)showAvatarBubble:(NSView*)anchor;

@end

@interface AvatarButtonController (ExposedForTesting)
- (BaseBubbleController*)menuController;
@end

#endif  
