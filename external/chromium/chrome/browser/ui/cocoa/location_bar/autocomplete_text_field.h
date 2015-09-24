// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOCOMPLETE_TEXT_FIELD_H_
#define CHROME_BROWSER_UI_COCOA_AUTOCOMPLETE_TEXT_FIELD_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/styled_text_field.h"
#import "chrome/browser/ui/cocoa/url_drop_target.h"

@class AutocompleteTextFieldCell;


class AutocompleteTextFieldObserver {
 public:
  
  virtual NSRange SelectionRangeForProposedRange(NSRange proposed_range) = 0;

  
  
  virtual void OnControlKeyChanged(bool pressed) = 0;

  
  virtual void OnPaste() = 0;

  
  virtual bool CanCopy() = 0;

  
  
  virtual void CopyToPasteboard(NSPasteboard* pboard) = 0;

  
  
  virtual bool CanPasteAndGo() = 0;

  
  
  
  
  virtual int GetPasteActionStringId() = 0;

  
  
  virtual void OnPasteAndGo() = 0;

  
  virtual void OnFrameChanged() = 0;

  
  
  virtual void ClosePopup() = 0;

  
  
  virtual void OnDidBeginEditing() = 0;
  virtual void OnBeforeChange() = 0;
  virtual void OnDidChange() = 0;
  virtual void OnDidEndEditing() = 0;

  
  virtual void OnStartingIME() = 0;

  
  
  
  
  
  
  
  
  
  virtual bool OnDoCommandBySelector(SEL cmd) = 0;

  
  virtual void OnSetFocus(bool control_down) = 0;

  
  virtual void OnKillFocus() = 0;

 protected:
  virtual ~AutocompleteTextFieldObserver() {}
};

@interface AutocompleteTextField : StyledTextField<NSTextViewDelegate,
                                                   URLDropTarget> {
 @private
  
  
  scoped_nsobject<NSUndoManager> undoManager_;

  AutocompleteTextFieldObserver* observer_;  

  
  scoped_nsobject<URLDropTargetHandler> dropHandler_;

  
  scoped_nsobject<NSMutableArray> currentToolTips_;
}

@property(nonatomic) AutocompleteTextFieldObserver* observer;

- (AutocompleteTextFieldCell*)cell;

- (void)setAttributedStringValue:(NSAttributedString*)aString;

- (void)clearUndoChain;

- (void)updateCursorAndToolTipRects;

- (NSMenu*)decorationMenuForEvent:(NSEvent*)event;

- (void)addToolTip:(NSString*)tooltip forRect:(NSRect)aRect;

@end

#endif  
