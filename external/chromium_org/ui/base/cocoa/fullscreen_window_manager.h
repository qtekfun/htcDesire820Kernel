// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_FULLSCREEN_WINDOW_MANAGER_H_
#define UI_BASE_COCOA_FULLSCREEN_WINDOW_MANAGER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/mac_util.h"
#include "base/mac/scoped_nsobject.h"
#include "ui/base/ui_export.h"

UI_EXPORT
@interface FullscreenWindowManager : NSObject {
 @private
  base::scoped_nsobject<NSWindow> window_;
  
  
  
  base::scoped_nsobject<NSScreen> desiredScreen_;
  base::mac::FullScreenMode fullscreenMode_;
  BOOL fullscreenActive_;
}

- (id)initWithWindow:(NSWindow*)window
       desiredScreen:(NSScreen*)desiredScreen;

- (void)enterFullscreenMode;

- (void)exitFullscreenMode;

@end

#endif  
