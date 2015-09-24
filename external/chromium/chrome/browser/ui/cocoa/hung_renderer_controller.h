// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_HUNG_RENDERER_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_HUNG_RENDERER_CONTROLLER_H_
#pragma once


#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#import "base/memory/scoped_nsobject.h"

@class MultiKeyEquivalentButton;
class TabContents;

@interface HungRendererController : NSWindowController<NSTableViewDataSource> {
 @private
  IBOutlet MultiKeyEquivalentButton* waitButton_;
  IBOutlet NSButton* killButton_;
  IBOutlet NSTableView* tableView_;
  IBOutlet NSImageView* imageView_;
  IBOutlet NSTextField* messageView_;

  
  
  TabContents* hungContents_;

  
  
  scoped_nsobject<NSArray> hungTitles_;

  
  
  scoped_nsobject<NSArray> hungFavicons_;
}

- (IBAction)kill:(id)sender;

- (IBAction)wait:(id)sender;

- (void)showForTabContents:(TabContents*)contents;

- (void)endForTabContents:(TabContents*)contents;

@end  


@interface HungRendererController (JustForTesting)
- (NSButton*)killButton;
- (MultiKeyEquivalentButton*)waitButton;
@end

#endif  
