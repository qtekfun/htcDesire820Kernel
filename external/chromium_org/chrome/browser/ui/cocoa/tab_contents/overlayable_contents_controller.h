// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_OVERLAYABLE_CONTENTS_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_OVERLAYABLE_CONTENTS_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"

class Browser;

namespace content {
class WebContents;
}

@interface OverlayableContentsController : NSViewController {
 @private
  
  base::scoped_nsobject<NSView> activeContainer_;
}

@property(readonly, nonatomic) NSView* activeContainer;

- (id)initWithBrowser:(Browser*)browser;

@end

#endif  
