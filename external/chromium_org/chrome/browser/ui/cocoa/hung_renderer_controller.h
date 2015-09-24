// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_HUNG_RENDERER_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_HUNG_RENDERER_CONTROLLER_H_


#import <Cocoa/Cocoa.h>

#import "base/mac/scoped_nsobject.h"
#import "base/memory/scoped_ptr.h"

@class MultiKeyEquivalentButton;
class WebContentsObserverBridge;

namespace content {
class WebContents;
}

@interface HungRendererController : NSWindowController<NSTableViewDataSource> {
 @private
  IBOutlet MultiKeyEquivalentButton* waitButton_;
  IBOutlet NSButton* killButton_;
  IBOutlet NSTableView* tableView_;
  IBOutlet NSImageView* imageView_;
  IBOutlet NSTextField* messageView_;

  
  
  content::WebContents* hungContents_;

  
  scoped_ptr<WebContentsObserverBridge> hungContentsObserver_;

  
  
  base::scoped_nsobject<NSArray> hungTitles_;

  
  
  base::scoped_nsobject<NSArray> hungFavicons_;
}

- (IBAction)kill:(id)sender;

- (IBAction)wait:(id)sender;

- (void)showForWebContents:(content::WebContents*)contents;

- (void)endForWebContents:(content::WebContents*)contents;

- (void)renderProcessGone;

@end  


@interface HungRendererController (JustForTesting)
- (NSButton*)killButton;
- (MultiKeyEquivalentButton*)waitButton;
@end

#endif  
