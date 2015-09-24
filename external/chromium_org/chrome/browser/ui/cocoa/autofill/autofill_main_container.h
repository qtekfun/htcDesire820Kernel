// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_MAIN_CONTAINER_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_MAIN_CONTAINER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/autofill/autofill_dialog_types.h"
#import "chrome/browser/ui/cocoa/autofill/autofill_layout.h"

@class AutofillDetailsContainer;
@class AutofillDialogWindowController;
@class AutofillNotificationContainer;
@class AutofillSectionContainer;
@class AutofillTooltipController;
@class GTMWidthBasedTweaker;
@class HyperlinkTextView;

namespace autofill {
  class AutofillDialogViewDelegate;
}

@interface AutofillMainContainer : NSViewController<AutofillLayout,
                                                    NSTextViewDelegate> {
 @private
  base::scoped_nsobject<GTMWidthBasedTweaker> buttonContainer_;
  base::scoped_nsobject<NSImageView> buttonStripImage_;
  base::scoped_nsobject<NSButton> saveInChromeCheckbox_;
  base::scoped_nsobject<AutofillTooltipController> saveInChromeTooltip_;
  base::scoped_nsobject<AutofillDetailsContainer> detailsContainer_;
  base::scoped_nsobject<HyperlinkTextView> legalDocumentsView_;
  base::scoped_nsobject<AutofillNotificationContainer> notificationContainer_;
  AutofillDialogWindowController* target_;

  
  autofill::AutofillDialogViewDelegate* delegate_;

  
  NSSize legalDocumentsSize_;

  
  BOOL legalDocumentsSizeDirty_;
}

@property(assign, nonatomic) AutofillDialogWindowController* target;

- (id)initWithDelegate:(autofill::AutofillDialogViewDelegate*)delegate;

- (NSSize)decorationSizeForWidth:(CGFloat)width;

- (void)setAnchorView:(NSView*)anchorView;

- (AutofillSectionContainer*)sectionForId:(autofill::DialogSection)section;

- (void)modelChanged;

- (BOOL)saveDetailsLocally;

- (void)updateLegalDocuments;

- (void)updateNotificationArea;

- (void)updateErrorBubble;

- (BOOL)validate;

- (void)updateSaveInChrome;

- (void)makeFirstInvalidInputFirstResponder;

- (void)scrollInitialEditorIntoViewAndMakeFirstResponder;

@end


@interface AutofillMainContainer (Testing)

@property(readonly, nonatomic) NSButton* saveInChromeCheckboxForTesting;
@property(readonly, nonatomic) NSImageView* buttonStripImageForTesting;
@property(readonly, nonatomic) NSImageView* saveInChromeTooltipForTesting;

@end

#endif  
