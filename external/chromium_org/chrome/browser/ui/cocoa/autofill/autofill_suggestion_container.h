// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_SUGGESTION_CONTAINER_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_SUGGESTION_CONTAINER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/autofill/autofill_layout.h"

namespace autofill {
  class AutofillDialogViewDelegate;
}

@class AutofillTextField;

@interface AutofillSuggestionContainer : NSViewController<AutofillLayout> {
 @private
  
  base::scoped_nsobject<NSBox> spacer_;

  
  base::scoped_nsobject<NSTextView> label_;

  
  base::scoped_nsobject<AutofillTextField> inputField_;

  autofill::AutofillDialogViewDelegate* delegate_;  
}

@property (readonly, nonatomic) AutofillTextField* inputField;

- (void)setSuggestionText:(NSString*)line icon:(NSImage*)icon;

- (void)showInputField:(NSString*)text withIcon:(NSImage*)icon;

@end

#endif  
