// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_INFOBARS_TRANSLATE_INFOBAR_BASE_H_
#define CHROME_BROWSER_UI_COCOA_INFOBARS_TRANSLATE_INFOBAR_BASE_H_

#import <Cocoa/Cocoa.h>

#import "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/translate/options_menu_model.h"
#include "chrome/browser/translate/translate_infobar_delegate.h"
#import "chrome/browser/ui/cocoa/infobars/infobar_controller.h"
#include "chrome/common/translate/translate_errors.h"

@interface TranslateInfoBarControllerBase : InfoBarController<NSMenuDelegate> {
 @protected
  base::scoped_nsobject<NSTextField> label1_;
  base::scoped_nsobject<NSTextField> label2_;
  base::scoped_nsobject<NSTextField> label3_;
  base::scoped_nsobject<NSPopUpButton> fromLanguagePopUp_;
  base::scoped_nsobject<NSPopUpButton> toLanguagePopUp_;
  base::scoped_nsobject<NSPopUpButton> optionsPopUp_;
  base::scoped_nsobject<NSButton> showOriginalButton_;
  
  
  
  base::scoped_nsobject<NSButton> translateMessageButton_;

  
  
  bool swappedLanguagePlaceholders_;

  
  CGFloat spaceBetweenControls_;

  scoped_ptr<OptionsMenuModel> optionsMenuModel_;
}

- (TranslateInfoBarDelegate*)delegate;

- (IBAction)showOriginal:(id)sender;

@end

@interface TranslateInfoBarControllerBase (ProtectedAPI)

- (void)adjustOptionsButtonSizeAndVisibilityForView:(NSView*)lastView;

- (void)layout;

- (void)loadLabelText;

- (NSArray*)visibleControls;

- (void)showVisibleControls:(NSArray*)visibleControls;

- (void)removeOkCancelButtons;

- (void)sourceLanguageModified:(NSInteger)newLanguageIdx;
- (void)targetLanguageModified:(NSInteger)newLanguageIdx;

- (void)languageMenuChanged:(id)item;
- (void)optionsMenuChanged:(id)item;

- (void)rebuildOptionsMenu:(BOOL)hideTitle;

- (BOOL)shouldShowOptionsPopUp;

@end 

#pragma mark TestingAPI

@interface TranslateInfoBarControllerBase (TestingAPI)

- (NSArray*)allControls;

- (bool)verifyLayout;

- (NSMenu*)optionsMenu;

- (NSButton*)translateMessageButton;

@end 


#endif 
