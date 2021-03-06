// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOCOMPLETE_TEXT_FIELD_H_
#define CHROME_BROWSER_UI_COCOA_AUTOCOMPLETE_TEXT_FIELD_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
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

  
  virtual bool ShouldEnableShowURL() = 0;

  
  virtual void ShowURL() = 0;

  
  
  virtual bool CanPasteAndGo() = 0;

  
  
  
  
  virtual int GetPasteActionStringId() = 0;

  
  
  virtual void OnPasteAndGo() = 0;

  
  virtual void OnFrameChanged() = 0;

  
  
  virtual void ClosePopup() = 0;

  
  
  virtual void OnDidBeginEditing() = 0;
  virtual void OnBeforeChange() = 0;
  virtual void OnDidChange() = 0;
  virtual void OnDidEndEditing() = 0;

  
  
  
  
  
  
  
  
  
  virtual bool OnDoCommandBySelector(SEL cmd) = 0;

  
  virtual void OnSetFocus(bool control_down) = 0;

  
  virtual void OnKillFocus() = 0;

  
  virtual void OnMouseDown(NSInteger button_number) = 0;

  
  virtual bool ShouldSelectAllOnMouseDown() = 0;

 protected:
  virtual ~AutocompleteTextFieldObserver() {}
};

@interface AutocompleteTextField : StyledTextField<NSTextViewDelegate,
                                                   URLDropTarget> {
 @private
  
  
  base::scoped_nsobject<NSUndoManager> undoManager_;

  AutocompleteTextFieldObserver* observer_;  

  
  base::scoped_nsobject<URLDropTargetHandler> dropHandler_;

  
  base::scoped_nsobject<NSMutableArray> currentToolTips_;

  base::scoped_nsobject<NSString> suggestText_;
  base::scoped_nsobject<NSColor> suggestColor_;
}

@property(nonatomic) AutocompleteTextFieldObserver* observer;

- (AutocompleteTextFieldCell*)cell;

- (void)setAttributedStringValue:(NSAttributedString*)aString;

- (void)clearUndoChain;

- (void)updateMouseTracking;

- (NSMenu*)decorationMenuForEvent:(NSEvent*)event;

- (void)addToolTip:(NSString*)tooltip forRect:(NSRect)aRect;

- (void)setGrayTextAutocompletion:(NSString*)suggestText
                        textColor:(NSColor*)suggestColor;

- (NSString*)suggestText;
- (NSColor*)suggestColor;

@end

namespace autocomplete_text_field {

void DrawGrayTextAutocompletion(NSAttributedString* mainText,
                                NSString* suggestText,
                                NSColor* suggestColor,
                                NSView* controlView,
                                NSRect frame);

}  

#endif  
