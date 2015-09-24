// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_DEV_TOOLS_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_DEV_TOOLS_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/devtools/devtools_window.h"

@class FocusTracker;
@class GraySplitView;
class Profile;

namespace content {
class WebContents;
}

@interface DevToolsController : NSObject<NSSplitViewDelegate> {
 @private
  
  base::scoped_nsobject<GraySplitView> splitView_;

  DevToolsDockSide dockSide_;

  
  
  DevToolsWindow* devToolsWindow_;

  base::scoped_nsobject<FocusTracker> focusTracker_;
}

- (id)init;

- (NSView*)view;

- (NSSplitView*)splitView;

- (void)updateDevToolsForWebContents:(content::WebContents*)contents
                         withProfile:(Profile*)profile;

@end

#endif  
