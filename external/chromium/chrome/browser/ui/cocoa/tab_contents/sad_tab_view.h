// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_SAD_TAB_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_SAD_TAB_VIEW_H_
#pragma once

#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/ui/cocoa/base_view.h"

#import <Cocoa/Cocoa.h>

@class HyperlinkButtonCell;

@interface SadTabView : BaseView {
 @private
  IBOutlet NSImageView* image_;
  IBOutlet NSTextField* title_;
  IBOutlet NSTextField* message_;
  IBOutlet NSButton* linkButton_;
  IBOutlet HyperlinkButtonCell* linkCell_;

  scoped_nsobject<NSColor> backgroundColor_;
  NSSize messageSize_;
}


- (void)removeLinkButton;

@end

#endif  
