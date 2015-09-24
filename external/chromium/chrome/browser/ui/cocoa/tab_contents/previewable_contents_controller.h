// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_PREVIEWABLE_CONTENTS_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_PREVIEWABLE_CONTENTS_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

class TabContents;

@interface PreviewableContentsController : NSViewController {
 @private
  
  IBOutlet NSView* activeContainer_;

  
  TabContents* previewContents_;  
}

@property(readonly, nonatomic) NSView* activeContainer;

- (void)showPreview:(TabContents*)preview;

- (void)hidePreview;

- (BOOL)isShowingPreview;

@end

#endif  
