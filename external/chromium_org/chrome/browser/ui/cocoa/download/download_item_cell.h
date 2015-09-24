// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_ITEM_CELL_H_
#define CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_ITEM_CELL_H_

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/gradient_button_cell.h"

#include "base/files/file_path.h"

class DownloadItemModel;
@class IndeterminateProgressTimer;


enum DownloadItemMousePosition {
  kDownloadItemMouseOutside,
  kDownloadItemMouseOverButtonPart,
  kDownloadItemMouseOverDropdownPart
};

@interface DownloadItemCell : GradientButtonCell<NSAnimationDelegate> {
 @private
  
  DownloadItemMousePosition mousePosition_;
  int mouseInsideCount_;
  base::scoped_nsobject<NSTrackingArea> trackingAreaButton_;
  base::scoped_nsobject<NSTrackingArea> trackingAreaDropdown_;

  base::FilePath downloadPath_;  
  NSString* secondaryTitle_;
  NSFont* secondaryFont_;
  int percentDone_;
  base::scoped_nsobject<NSAnimation> completionAnimation_;

  
  int indeterminateProgressAngle_;
  base::scoped_nsobject<IndeterminateProgressTimer> indeterminateProgressTimer_;

  BOOL isStatusTextVisible_;
  CGFloat titleY_;
  CGFloat statusAlpha_;
  base::scoped_nsobject<NSAnimation> toggleStatusVisibilityAnimation_;

  scoped_ptr<ui::ThemeProvider> themeProvider_;
}

@property(nonatomic, copy) NSString* secondaryTitle;
@property(nonatomic, retain) NSFont* secondaryFont;

- (void)setStateFromDownload:(DownloadItemModel*)downloadModel;

- (BOOL)isMouseOverButtonPart;

@end

@interface DownloadItemCell(TestingAPI)
- (BOOL)isStatusTextVisible;
- (CGFloat)statusTextAlpha;
- (void)skipVisibilityAnimation;
- (void)showSecondaryTitle;
- (void)hideSecondaryTitle;
- (IndeterminateProgressTimer*)indeterminateProgressTimer;
@end

#endif  
