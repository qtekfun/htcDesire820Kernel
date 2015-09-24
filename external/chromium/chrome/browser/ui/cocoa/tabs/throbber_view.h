// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_THROBBER_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_TABS_THROBBER_VIEW_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"

@protocol ThrobberDataDelegate;


@interface ThrobberView : NSView {
 @private
  id<ThrobberDataDelegate> dataDelegate_;
}

+ (id)filmstripThrobberViewWithFrame:(NSRect)frame
                               image:(NSImage*)image;

+ (id)toastThrobberViewWithFrame:(NSRect)frame
                     beforeImage:(NSImage*)beforeImage
                      afterImage:(NSImage*)afterImage;

@end

#endif  
