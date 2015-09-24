// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_DETAILS_CONTAINER_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_DETAILS_CONTAINER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/autofill/autofill_dialog_types.h"
#import "chrome/browser/ui/cocoa/autofill/autofill_layout.h"
#import "chrome/browser/ui/cocoa/autofill/autofill_section_container.h"


namespace autofill {
class AutofillDialogViewDelegate;
}

@class InfoBubbleView;
@class AutofillBubbleController;

@interface AutofillDetailsContainer
    : NSViewController<AutofillLayout,
                       AutofillValidationDisplay> {
 @private
  
  base::scoped_nsobject<NSScrollView> scrollView_;

  
  base::scoped_nsobject<NSMutableArray> details_;

  
  base::scoped_nsobject<InfoBubbleView> errorBubble_;

  AutofillBubbleController* errorBubbleController_;

  autofill::AutofillDialogViewDelegate* delegate_;  
}

- (id)initWithDelegate:(autofill::AutofillDialogViewDelegate*)delegate;

- (AutofillSectionContainer*)sectionForId:(autofill::DialogSection)section;

- (void)updateErrorBubble;

- (void)modelChanged;

- (BOOL)validate;

- (NSControl*)firstInvalidField;

- (NSControl*)firstVisibleField;

- (void)scrollToView:(NSView*)field;

@end

#endif  
