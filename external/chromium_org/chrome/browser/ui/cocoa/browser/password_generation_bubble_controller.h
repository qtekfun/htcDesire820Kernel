// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_PASSWORD_GENERATION_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_PASSWORD_GENERATION_BUBBLE_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/ui/cocoa/base_bubble_controller.h"
#import "chrome/browser/ui/cocoa/styled_text_field.h"
#include "components/autofill/core/common/password_form.h"
#include "components/autofill/core/common/password_generation_util.h"

namespace autofill {
class PasswordGenerator;
}

namespace content {
class RenderViewHost;
}

class Browser;
class PasswordManager;
@class PasswordGenerationBubbleController;

@interface PasswordGenerationTextField : StyledTextField {
}

- (id)initWithFrame:(NSRect)frame
     withController:(PasswordGenerationBubbleController*)controller
        normalImage:(NSImage*)normalImage
         hoverImage:(NSImage*)hoverImage;
@end

@interface PasswordGenerationBubbleController :
    BaseBubbleController<NSTextFieldDelegate> {
 @private
  
  content::RenderViewHost* renderViewHost_;
  PasswordManager* passwordManager_;
  autofill::PasswordGenerator* passwordGenerator_;
  autofill::PasswordForm form_;
  autofill::password_generation::PasswordGenerationActions actions_;

  PasswordGenerationTextField* textField_;   
}

@property(readonly) PasswordGenerationTextField* textField;

- (id)initWithWindow:(NSWindow*)parentWindow
          anchoredAt:(NSPoint)point
      renderViewHost:(content::RenderViewHost*)renderViewHost
     passwordManager:(PasswordManager*)passwordManager
      usingGenerator:(autofill::PasswordGenerator*)passwordGenerator
             forForm:(const autofill::PasswordForm&)form;
- (void)performLayout;
- (IBAction)fillPassword:(id)sender;
- (void)regeneratePassword;
@end


@interface PasswordGenerationTextField (ExposedForTesting)
- (void)simulateIconClick;
@end

#endif  