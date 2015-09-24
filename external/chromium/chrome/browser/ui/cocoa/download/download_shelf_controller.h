// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/view_resizer.h"

@class AnimatableView;
class BaseDownloadItemModel;
class Browser;
@class BrowserWindowController;
@class DownloadItemController;
class DownloadShelf;
@class DownloadShelfView;
@class HyperlinkButtonCell;
@class HoverButton;


@interface DownloadShelfController : NSViewController<NSTextViewDelegate> {
 @private
  IBOutlet HyperlinkButtonCell* showAllDownloadsCell_;

  IBOutlet NSImageView* image_;

  IBOutlet HoverButton* hoverCloseButton_;

  BOOL barIsVisible_;

  BOOL isFullscreen_;

  scoped_ptr<DownloadShelf> bridge_;

  
  CGFloat maxShelfHeight_;

  
  
  CGFloat currentShelfHeight_;

  
  
  
  scoped_nsobject<NSTrackingArea> trackingArea_;

  
  scoped_nsobject<NSMutableArray> downloadItemControllers_;

  
  IBOutlet NSView* itemContainerView_;

  
  id<ViewResizer> resizeDelegate_;
};

- (id)initWithBrowser:(Browser*)browser
       resizeDelegate:(id<ViewResizer>)resizeDelegate;

- (IBAction)showDownloadsTab:(id)sender;

- (AnimatableView*)animatableView;

- (DownloadShelf*)bridge;
- (BOOL)isVisible;

- (IBAction)show:(id)sender;

- (IBAction)hide:(id)sender;

- (void)addDownloadItem:(BaseDownloadItemModel*)model;

- (void)remove:(DownloadItemController*)download;

- (void)downloadWasOpened:(DownloadItemController*)download;

- (void)exiting;

- (float)height;

- (void)layoutItems;

@end
