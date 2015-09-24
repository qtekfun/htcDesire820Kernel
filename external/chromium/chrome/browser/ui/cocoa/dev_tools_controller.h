// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_DEV_TOOLS_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_DEV_TOOLS_CONTROLLER_H_
#pragma once

#import <Foundation/Foundation.h>

#include "base/memory/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/tab_contents/tab_contents_controller.h"

@class NSSplitView;
@class NSView;

class Profile;
class TabContents;

@interface DevToolsController : NSObject {
 @private
  
  scoped_nsobject<NSSplitView> splitView_;

  
  scoped_nsobject<TabContentsController> contentsController_;
}

- (id)initWithDelegate:(id<TabContentsControllerDelegate>)delegate;

- (NSView*)view;

- (NSSplitView*)splitView;

- (void)updateDevToolsForTabContents:(TabContents*)contents
                         withProfile:(Profile*)profile;

- (void)ensureContentsVisible;

@end

#endif  
