// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_ITEM_CELL_H_
#define CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_ITEM_CELL_H_
#pragma once

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/gradient_button_cell.h"

#include "base/file_path.h"

class BaseDownloadItemModel;


enum DownloadItemMousePosition {
  kDownloadItemMouseOutside,
  kDownloadItemMouseOverButtonPart,
  kDownloadItemMouseOverDropdownPart
};

@interface DownloadItemCell : GradientButtonCell<NSAnimationDelegate> {
 @private
  
  DownloadItemMousePosition mousePosition_;
  int mouseInsideCount_;
  scoped_nsobject<NSTrackingArea> trackingAreaButton_;
  scoped_nsobject<NSTrackingArea> trackingAreaDropdown_;

  FilePath downloadPath_;  
  NSString* secondaryTitle_;
  NSFont* secondaryFont_;
  int percentDone_;
  scoped_nsobject<NSAnimation> completionAnimation_;

  BOOL isStatusTextVisible_;
  CGFloat titleY_;
  CGFloat statusAlpha_;
  scoped_nsobject<NSAnimation> hideStatusAnimation_;

  scoped_ptr<ui::ThemeProvider> themeProvider_;
}

@property(nonatomic, copy) NSString* secondaryTitle;
@property(nonatomic, retain) NSFont* secondaryFont;

- (void)setStateFromDownload:(BaseDownloadItemModel*)downloadModel;

- (BOOL)isMouseOverButtonPart;

@end

#endif  
