// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Cocoa/Cocoa.h>
#import "chrome/browser/ui/cocoa/chrome_browser_window.h"

@interface FullscreenWindow : ChromeBrowserWindow

- (id)initForScreen:(NSScreen*)screen;

@end
