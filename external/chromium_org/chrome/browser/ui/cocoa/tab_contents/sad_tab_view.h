// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_SAD_TAB_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_SAD_TAB_VIEW_H_

#include "base/mac/scoped_nsobject.h"
#include "ui/base/cocoa/base_view.h"

#import <Cocoa/Cocoa.h>

@class SadTabController;
@class HyperlinkTextView;

@interface SadTabView : BaseView<NSTextViewDelegate> {
 @private
  IBOutlet NSImageView* image_;
  IBOutlet NSTextField* title_;
  IBOutlet NSTextField* message_;
  IBOutlet NSTextField* helpPlaceholder_;

  base::scoped_nsobject<NSColor> backgroundColor_;
  NSSize messageSize_;

  
  
  
  
  base::scoped_nsobject<HyperlinkTextView> help_;

  
  IBOutlet SadTabController* controller_;
}


- (void)removeHelpText;

- (void)initializeHelpText;

@end

#endif  
