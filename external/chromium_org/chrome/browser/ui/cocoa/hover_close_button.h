// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_HOVER_CLOSE_BUTTON_H_
#define CHROME_BROWSER_UI_COCOA_HOVER_CLOSE_BUTTON_H_

#import <Cocoa/Cocoa.h>

#import "ui/base/cocoa/hover_button.h"

@class GTMKeyValueAnimation;

@interface HoverCloseButton : HoverButton<NSAnimationDelegate> {
 @private
  GTMKeyValueAnimation* fadeOutAnimation_;
  HoverState previousState_;
}

@end

@interface WebUIHoverCloseButton : HoverCloseButton
@end

#endif  
