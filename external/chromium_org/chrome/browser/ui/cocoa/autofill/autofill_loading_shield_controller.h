// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_LOADING_SHIELD_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_LOADING_SHIELD_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/autofill/autofill_layout.h"

class AutofillLoadingAnimationBridge;

namespace autofill {
class AutofillDialogViewDelegate;
class LoadingAnimation;
}

@interface AutofillLoadingShieldController : NSViewController<AutofillLayout> {
 @private
  
  base::scoped_nsobject<NSTextField> message_;

  
  base::scoped_nsobject<NSArray> dots_;

  
  scoped_ptr<AutofillLoadingAnimationBridge> animationDriver_;

  autofill::AutofillDialogViewDelegate* delegate_;  
}

- (id)initWithDelegate:(autofill::AutofillDialogViewDelegate*)delegate;

- (void)update;

- (void)relayoutDotsForSteppedAnimation:
    (const autofill::LoadingAnimation&)animation;

@end

#endif  
