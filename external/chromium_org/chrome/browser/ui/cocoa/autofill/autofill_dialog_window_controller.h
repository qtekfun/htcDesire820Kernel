// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_DIALOG_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_DIALOG_WINDOW_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/autofill/autofill_dialog_types.h"
#import "chrome/browser/ui/cocoa/autofill/autofill_layout.h"

@class AutofillHeader;
@class AutofillLoadingShieldController;
@class AutofillMainContainer;
@class AutofillOverlayController;
@class AutofillSignInContainer;

namespace content {
class NavigationController;
class WebContents;
}  

namespace autofill {
class AutofillDialogCocoa;
}  


@protocol AutofillDialogBridge

- (void)show;
- (void)hide;
- (void)updateNotificationArea;
- (void)updateAccountChooser;
- (void)updateButtonStrip;
- (void)updateSection:(autofill::DialogSection)section;
- (void)updateForErrors;
- (void)fillSection:(autofill::DialogSection)section
           forInput:(const autofill::DetailInput&)input;
- (void)getInputs:(autofill::FieldValueMap*)outputs
       forSection:(autofill::DialogSection)section;
- (NSString*)getCvc;
- (BOOL)saveDetailsLocally;
- (content::NavigationController*)showSignIn;
- (void)hideSignIn;
- (void)modelChanged;
- (void)updateErrorBubble;
- (void)onSignInResize:(NSSize)size;

@end


@interface AutofillDialogWindowController :
    NSWindowController<NSWindowDelegate, AutofillLayout, AutofillDialogBridge> {
 @private
  content::WebContents* webContents_;  
  autofill::AutofillDialogCocoa* dialog_;  

  base::scoped_nsobject<AutofillHeader> header_;
  base::scoped_nsobject<AutofillMainContainer> mainContainer_;
  base::scoped_nsobject<AutofillSignInContainer> signInContainer_;
  base::scoped_nsobject<AutofillOverlayController> overlayController_;
  base::scoped_nsobject<AutofillLoadingShieldController>
      loadingShieldController_;
  base::scoped_nsobject<NSTextView> fieldEditor_;

  
  BOOL mainContainerBecameVisible_;
}

- (id)initWithWebContents:(content::WebContents*)webContents
                   dialog:(autofill::AutofillDialogCocoa*)dialog;

- (void)requestRelayout;

- (IBAction)accept:(id)sender;

- (IBAction)cancel:(id)sender;

@end


@interface AutofillDialogWindowController (TestableAutofillDialogView)

- (void)setTextContents:(NSString*)text
               forInput:(const autofill::DetailInput&)input;
- (void)setTextContents:(NSString*)text
 ofSuggestionForSection:(autofill::DialogSection)section;
- (void)activateFieldForInput:(const autofill::DetailInput&)input;
- (content::WebContents*)getSignInWebContents;
- (BOOL)isShowingOverlay;

@end

#endif  
