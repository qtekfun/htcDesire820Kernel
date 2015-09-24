// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "base/time.h"

class BaseDownloadItemModel;
@class ChromeUILocalizer;
@class DownloadItemCell;
class DownloadItem;
@class DownloadItemButton;
class DownloadItemMac;
class DownloadShelfContextMenuMac;
@class DownloadShelfController;
@class GTMWidthBasedTweaker;


@interface DownloadItemController : NSViewController {
 @private
  IBOutlet DownloadItemButton* progressView_;
  IBOutlet DownloadItemCell* cell_;

  IBOutlet NSMenu* activeDownloadMenu_;
  IBOutlet NSMenu* completeDownloadMenu_;

  
  IBOutlet NSView* dangerousDownloadView_;
  IBOutlet NSTextField* dangerousDownloadLabel_;
  IBOutlet NSButton* dangerousDownloadConfirmButton_;

  
  
  IBOutlet GTMWidthBasedTweaker* buttonTweaker_;

  
  
  
  
  IBOutlet ChromeUILocalizer* localizer_;

  IBOutlet NSImageView* image_;

  scoped_ptr<DownloadItemMac> bridge_;
  scoped_ptr<DownloadShelfContextMenuMac> menuBridge_;

  
  DownloadShelfController* shelf_;

  
  base::Time creationTime_;

  
  enum DownoadItemState {
    kNormal,
    kDangerous
  } state_;
};

- (id)initWithModel:(BaseDownloadItemModel*)downloadModel
              shelf:(DownloadShelfController*)shelf;

- (void)setStateFromDownload:(BaseDownloadItemModel*)downloadModel;

- (void)remove;

- (void)updateVisibility:(id)sender;

- (void)downloadWasOpened;

- (void)setIcon:(NSImage*)icon;

- (IBAction)handleButtonClick:(id)sender;

- (NSSize)preferredSize;

- (DownloadItem*)download;

- (void)updateToolTip;

- (void)clearDangerousMode;
- (BOOL)isDangerousMode;
- (IBAction)saveDownload:(id)sender;
- (IBAction)discardDownload:(id)sender;

- (IBAction)handleOpen:(id)sender;
- (IBAction)handleAlwaysOpen:(id)sender;
- (IBAction)handleReveal:(id)sender;
- (IBAction)handleCancel:(id)sender;
- (IBAction)handleTogglePause:(id)sender;

@end
