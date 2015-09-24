// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_OMNIBOX_OMNIBOX_POPUP_MATRIX_H_
#define CHROME_BROWSER_UI_COCOA_OMNIBOX_OMNIBOX_POPUP_MATRIX_H_

#import <Cocoa/Cocoa.h>

#import "ui/base/cocoa/tracking_area.h"
#include "ui/base/window_open_disposition.h"

@class OmniboxPopupMatrix;

class OmniboxPopupMatrixDelegate {
 public:
  
  virtual void OnMatrixRowSelected(OmniboxPopupMatrix* matrix, size_t row) = 0;

  
  virtual void OnMatrixRowClicked(OmniboxPopupMatrix* matrix, size_t row) = 0;

  
  virtual void OnMatrixRowMiddleClicked(OmniboxPopupMatrix* matrix,
                                        size_t row) = 0;
};

@interface OmniboxPopupMatrix : NSMatrix {
  OmniboxPopupMatrixDelegate* delegate_;  
  ui::ScopedCrTrackingArea trackingArea_;
}

- (id)initWithDelegate:(OmniboxPopupMatrixDelegate*)delegate;

- (void)setDelegate:(OmniboxPopupMatrixDelegate*)delegate;

- (NSInteger)highlightedRow;

@end

#endif  
