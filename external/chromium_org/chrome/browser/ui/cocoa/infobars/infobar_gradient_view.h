// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_GRADIENT_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_GRADIENT_VIEW_H_

#include "chrome/browser/infobars/infobar_delegate.h"
#import "chrome/browser/ui/cocoa/vertical_gradient_view.h"

#import <Cocoa/Cocoa.h>

@interface InfoBarGradientView : VerticalGradientView {
 @private
  CGFloat arrowHeight_;
  CGFloat arrowHalfWidth_;
  CGFloat arrowX_;
  BOOL hasTip_;
}

@property(assign, nonatomic) CGFloat arrowHeight;
@property(assign, nonatomic) CGFloat arrowHalfWidth;
@property(assign, nonatomic) CGFloat arrowX;
@property(assign, nonatomic) BOOL hasTip;

- (void)setInfobarType:(InfoBarDelegate::Type)infobarType;

@end

#endif  