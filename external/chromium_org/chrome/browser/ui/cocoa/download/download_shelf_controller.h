// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/view_resizer.h"
#include "ui/base/cocoa/tracking_area.h"

@class AnimatableView;
class Browser;
@class BrowserWindowController;
@class DownloadItemController;
class DownloadShelf;
@class DownloadShelfView;
@class HyperlinkButtonCell;
@class HoverButton;

namespace content {
class DownloadItem;
class PageNavigator;
}


@interface DownloadShelfController : NSViewController<NSTextViewDelegate> {
 @private
  IBOutlet HoverButton* hoverCloseButton_;

  
  
  
  BOOL barIsVisible_;

  
  BOOL isFullscreen_;

  
  BOOL shouldCloseOnMouseExit_;

  
  BOOL isMouseInsideView_;

  scoped_ptr<DownloadShelf> bridge_;

  
  CGFloat maxShelfHeight_;

  
  
  CGFloat currentShelfHeight_;

  
  ui::ScopedCrTrackingArea trackingArea_;

  
  base::scoped_nsobject<NSMutableArray> downloadItemControllers_;

  
  IBOutlet NSView* itemContainerView_;

  
  id<ViewResizer> resizeDelegate_;

  
  content::PageNavigator* navigator_;
};

- (id)initWithBrowser:(Browser*)browser
       resizeDelegate:(id<ViewResizer>)resizeDelegate;

- (IBAction)showDownloadsTab:(id)sender;

- (IBAction)handleClose:(id)sender;

- (void)showDownloadShelf:(BOOL)show
             isUserAction:(BOOL)isUserAction;

- (AnimatableView*)animatableView;

- (DownloadShelf*)bridge;
- (BOOL)isVisible;

- (void)addDownloadItem:(content::DownloadItem*)downloadItem;

- (void)add:(DownloadItemController*)download;

- (void)remove:(DownloadItemController*)download;

- (void)downloadWasOpened:(DownloadItemController*)download;

- (void)exiting;

- (float)height;

- (void)layoutItems;

@end
