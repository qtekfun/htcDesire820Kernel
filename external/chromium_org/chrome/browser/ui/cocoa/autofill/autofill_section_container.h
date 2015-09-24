// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_SECTION_CONTAINER_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_SECTION_CONTAINER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/autofill/autofill_dialog_types.h"
#import "chrome/browser/ui/cocoa/autofill/autofill_input_field.h"
#import "chrome/browser/ui/cocoa/autofill/autofill_layout.h"

namespace autofill {
class AutofillDialogViewDelegate;
}

@class AutofillSectionView;
@class AutofillSuggestionContainer;
@class AutofillTextField;
@class AutofillTooltipController;
@class LayoutView;
@class MenuButton;
@class MenuController;

@protocol AutofillValidationDisplay

- (void)updateMessageForField:(NSControl<AutofillInputField>*)field;

- (void)hideErrorBubble;

@end


@interface AutofillSectionContainer :
    NSViewController<AutofillLayout, AutofillInputDelegate> {
 @private
  base::scoped_nsobject<LayoutView> inputs_;
  base::scoped_nsobject<MenuButton> suggestButton_;
  base::scoped_nsobject<AutofillSuggestionContainer> suggestContainer_;
  base::scoped_nsobject<NSTextField> label_;

  
  base::scoped_nsobject<AutofillSectionView> view_;

  
  
  base::scoped_nsobject<AutofillTooltipController> tooltipController_;

  
  
  AutofillTextField* tooltipField_;

  
  std::vector<const autofill::DetailInput*> detailInputs_;

  
  id<AutofillValidationDisplay> validationDelegate_;

  
  
  BOOL showSuggestions_;

  base::scoped_nsobject<MenuController> menuController_;
  autofill::DialogSection section_;
  autofill::AutofillDialogViewDelegate* delegate_;  
}

@property(readonly, nonatomic) autofill::DialogSection section;
@property(assign, nonatomic) id<AutofillValidationDisplay> validationDelegate;

- (id)initWithDelegate:(autofill::AutofillDialogViewDelegate*)delegate
            forSection:(autofill::DialogSection)section;

- (void)getInputs:(autofill::FieldValueMap*)output;

- (void)modelChanged;

- (void)update;

- (void)fillForInput:(const autofill::DetailInput&)input;

- (BOOL)validateFor:(autofill::ValidationType)validationType;

- (NSString*)suggestionText;

- (void)addInputsToArray:(NSMutableArray*)array;

@end

@interface AutofillSectionContainer (ForTesting)

- (NSControl<AutofillInputField>*)getField:(autofill::ServerFieldType)type;

- (void)setFieldValue:(NSString*)text
             forInput:(const autofill::DetailInput&)input;

- (void)setSuggestionFieldValue:(NSString*)text;

- (void)activateFieldForInput:(const autofill::DetailInput&)input;

@end

#endif  
