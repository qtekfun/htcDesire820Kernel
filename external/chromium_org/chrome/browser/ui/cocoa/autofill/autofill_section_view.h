// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_SECTION_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_SECTION_VIEW_H_

#import <Cocoa/Cocoa.h>

#import "ui/base/cocoa/base_view.h"

@interface AutofillSectionView : BaseView {
 @private
  NSControl* clickTarget_;  
  BOOL isHighlighted_;  
  BOOL shouldHighlightOnHover_;  
}

@property(assign, nonatomic) NSControl* clickTarget;

@property(readonly, nonatomic, getter=hoverColor) NSColor* hoverColor;

@property(assign, nonatomic) BOOL shouldHighlightOnHover;

@property(readonly, nonatomic) BOOL isHighlighted;

@end

#endif  
