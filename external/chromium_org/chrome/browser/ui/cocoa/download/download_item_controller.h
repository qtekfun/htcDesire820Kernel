// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"

@class ChromeUILocalizer;
@class DownloadItemCell;
@class DownloadItemButton;
class DownloadItemMac;
class DownloadItemModel;
class DownloadShelfContextMenuMac;
@class DownloadShelfController;
@class GTMWidthBasedTweaker;

namespace content {
class DownloadItem;
class PageNavigator;
}

namespace gfx {
class FontList;
}

namespace ui {
class MenuModel;
}


@interface DownloadItemController : NSViewController {
 @private
  IBOutlet DownloadItemButton* progressView_;
  IBOutlet DownloadItemCell* cell_;

  
  IBOutlet NSView* dangerousDownloadView_;
  IBOutlet NSTextField* dangerousDownloadLabel_;
  IBOutlet NSButton* dangerousDownloadConfirmButton_;

  
  
  IBOutlet GTMWidthBasedTweaker* dangerousButtonTweaker_;
  IBOutlet GTMWidthBasedTweaker* maliciousButtonTweaker_;

  
  
  
  
  IBOutlet ChromeUILocalizer* localizer_;

  IBOutlet NSImageView* image_;

  scoped_ptr<DownloadItemMac> bridge_;
  scoped_ptr<DownloadShelfContextMenuMac> menuBridge_;

  
  DownloadShelfController* shelf_;

  
  base::Time creationTime_;

  
  scoped_ptr<gfx::FontList> font_list_;

  
  enum DownoadItemState {
    kNormal,
    kDangerous
  } state_;
};

- (id)initWithDownload:(content::DownloadItem*)downloadItem
                 shelf:(DownloadShelfController*)shelf
             navigator:(content::PageNavigator*)navigator;

- (void)setStateFromDownload:(DownloadItemModel*)downloadModel;

- (void)remove;

- (void)updateVisibility:(id)sender;

- (void)downloadWasOpened;

- (void)setIcon:(NSImage*)icon;

- (IBAction)handleButtonClick:(id)sender;

- (NSSize)preferredSize;

- (content::DownloadItem*)download;

- (ui::MenuModel*)contextMenuModel;

- (void)updateToolTip;

- (void)clearDangerousMode;
- (BOOL)isDangerousMode;
- (IBAction)saveDownload:(id)sender;
- (IBAction)discardDownload:(id)sender;
- (IBAction)dismissMaliciousDownload:(id)sender;
- (IBAction)showContextMenu:(id)sender;
@end
