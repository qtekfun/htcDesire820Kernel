// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TRANSLATE_INFOBAR_BASE_H_
#define CHROME_BROWSER_UI_COCOA_TRANSLATE_INFOBAR_BASE_H_
#pragma once

#import <Cocoa/Cocoa.h>
#import "chrome/browser/ui/cocoa/infobars/infobar_controller.h"

#import "base/mac/cocoa_protocols.h"
#import "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/translate/languages_menu_model.h"
#include "chrome/browser/translate/options_menu_model.h"
#include "chrome/browser/translate/translate_infobar_delegate.h"
#include "chrome/common/translate_errors.h"

class TranslateInfoBarMenuModel;

#pragma mark TranslateInfoBarUtilities helper functions.
namespace TranslateInfoBarUtilities {

void MoveControl(NSView* anchor, NSView* toMove, int spacing, bool after);

void VerticallyCenterView(NSView *toMove);
bool VerifyControlOrderAndSpacing(id before, id after);

NSTextField* CreateLabel(NSRect bounds);

void AddMenuItem(NSMenu *menu, id target, SEL selector, NSString* title,
    int tag, bool enabled, bool checked);

}  

@interface TranslateInfoBarControllerBase : InfoBarController<NSMenuDelegate> {
 @protected
  scoped_nsobject<NSTextField> label1_;
  scoped_nsobject<NSTextField> label2_;
  scoped_nsobject<NSTextField> label3_;
  scoped_nsobject<NSPopUpButton> fromLanguagePopUp_;
  scoped_nsobject<NSPopUpButton> toLanguagePopUp_;
  scoped_nsobject<NSPopUpButton> optionsPopUp_;
  scoped_nsobject<NSButton> showOriginalButton_;
  
  
  
  scoped_nsobject<NSButton> translateMessageButton_;

  
  
  bool swappedLanguagePlaceholders_;

  
  CGFloat spaceBetweenControls_;

  scoped_ptr<LanguagesMenuModel> originalLanguageMenuModel_;
  scoped_ptr<LanguagesMenuModel> targetLanguageMenuModel_;
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
