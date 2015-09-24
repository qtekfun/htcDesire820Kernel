// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_TAB_CONTENTS_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_TAB_CONTENTS_CONTROLLER_H_
#pragma once

#include <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"

class TabContents;
class TabContentsNotificationBridge;
@class TabContentsController;


@protocol TabContentsControllerDelegate

- (void)tabContentsViewFrameWillChange:(TabContentsController*)source
                             frameRect:(NSRect)frameRect;

@end


@interface TabContentsController : NSViewController {
 @private
  TabContents* contents_;  
  
  id<TabContentsControllerDelegate> delegate_;  
  scoped_ptr<TabContentsNotificationBridge> tabContentsBridge_;
}
@property(readonly, nonatomic) TabContents* tabContents;

- (id)initWithContents:(TabContents*)contents
              delegate:(id<TabContentsControllerDelegate>)delegate;

- (void)ensureContentsSizeDoesNotChange;

- (void)ensureContentsVisible;

- (void)changeTabContents:(TabContents*)newContents;

- (void)willBecomeUnselectedTab;

- (void)willBecomeSelectedTab;

- (void)tabDidChange:(TabContents*)updatedContents;

@end

#endif  
