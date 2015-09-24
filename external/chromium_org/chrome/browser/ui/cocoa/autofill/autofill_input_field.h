// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_INPUT_FIELD_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_INPUT_FIELD_H_

#import <Cocoa/Cocoa.h>

@protocol AutofillInputField;

@protocol AutofillInputCell<NSObject>

@property(nonatomic, assign) BOOL invalid;
@property(nonatomic, copy) NSString* fieldValue;
@property(nonatomic, copy) NSString* defaultValue;

@end

@protocol AutofillInputDelegate<NSObject>

enum KeyEventHandled {
  kKeyEventNotHandled,
  kKeyEventHandled
};

- (KeyEventHandled)keyEvent:(NSEvent*)event forInput:(id)sender;

- (void)onMouseDown:(NSControl<AutofillInputField>*)sender;

- (void)fieldBecameFirstResponder:(NSControl<AutofillInputField>*)field;

- (void)didChange:(id)sender;

- (void)didEndEditing:(id)sender;

@end

@protocol AutofillInputField

@property(nonatomic, assign) id<AutofillInputDelegate> inputDelegate;

@property(nonatomic, copy) NSString* fieldValue;
@property(nonatomic, copy) NSString* defaultValue;

@property(nonatomic, readonly) BOOL isDefault;

@property(nonatomic, copy) NSString* validityMessage;

@property(nonatomic, readonly) BOOL invalid;

@end

#endif  
