// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_NOTIFICATION_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_NOTIFICATION_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/autofill/autofill_dialog_types.h"
#import "chrome/browser/ui/cocoa/autofill/autofill_layout.h"
#include "url/gurl.h"

@class AutofillTooltipController;
@class HyperlinkTextView;

namespace autofill {
class AutofillDialogViewDelegate;
}

@interface AutofillNotificationController :
    NSViewController<AutofillLayout, NSTextViewDelegate> {
 @private
  
  base::scoped_nsobject<HyperlinkTextView> textview_;

  
  base::scoped_nsobject<NSButton> checkbox_;

  
  base::scoped_nsobject<AutofillTooltipController> tooltipController_;

  
  GURL linkURL_;

  
  autofill::DialogNotification::Type notificationType_;

  
  autofill::AutofillDialogViewDelegate* delegate_;
}

@property(nonatomic, readonly) NSTextView* textview;
@property(nonatomic, readonly) NSButton* checkbox;
@property(nonatomic, readonly) NSView* tooltipView;

- (id)initWithNotification:(const autofill::DialogNotification*)notification
                  delegate:(autofill::AutofillDialogViewDelegate*)delegate;

- (void)setHasArrow:(BOOL)hasArrow withAnchorView:(NSView*)anchorView;

- (BOOL)hasArrow;

- (NSSize)preferredSizeForWidth:(CGFloat)width;

- (IBAction)checkboxClicked:(id)sender;

@end

#endif  
