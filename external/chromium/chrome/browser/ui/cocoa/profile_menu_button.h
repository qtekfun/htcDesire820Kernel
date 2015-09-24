// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_PROFILE_MENU_BUTTON_H_
#define CHROME_BROWSER_UI_COCOA_PROFILE_MENU_BUTTON_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"

@interface ProfileMenuButton : NSPopUpButton {
 @private
  BOOL shouldShowProfileDisplayName_;
  scoped_nsobject<NSTextFieldCell> textFieldCell_;

  scoped_nsobject<NSImage> cachedTabImage_;
  
  
  BOOL cachedTabImageIsPressed_;
}

@property(assign,nonatomic) BOOL shouldShowProfileDisplayName;
@property(assign,nonatomic) NSString* profileDisplayName;

- (NSSize)desiredControlSize;
- (NSSize)minControlSize;

- (void)   mouseDown:(NSEvent*)event
  withShowMenuTarget:(id)target;

@end

#endif  
