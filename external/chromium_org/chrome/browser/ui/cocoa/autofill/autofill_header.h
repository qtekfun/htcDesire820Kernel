// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_HEADER_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_HEADER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/autofill/autofill_layout.h"

namespace autofill {
class AutofillDialogViewDelegate;
}  

@class AutofillAccountChooser;

@interface AutofillHeader : NSViewController<AutofillLayout> {
 @private
  base::scoped_nsobject<AutofillAccountChooser> accountChooser_;
  base::scoped_nsobject<NSTextField> title_;
  

  autofill::AutofillDialogViewDelegate* delegate_;  
}

- (id)initWithDelegate:(autofill::AutofillDialogViewDelegate*)delegate;

- (NSView*)anchorView;

- (void)update;

- (CGFloat)heightForWidth:(int)width;

@end

#endif  
