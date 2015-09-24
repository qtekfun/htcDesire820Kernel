// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_SAD_TAB_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_SAD_TAB_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

class TabContents;

@interface SadTabController : NSViewController {
 @private
  TabContents* tabContents_;  
}

- (id)initWithTabContents:(TabContents*)someTabContents
                superview:(NSView*)superview;

- (IBAction)openLearnMoreAboutCrashLink:(id)sender;

- (TabContents*)tabContents;

@end

#endif  
