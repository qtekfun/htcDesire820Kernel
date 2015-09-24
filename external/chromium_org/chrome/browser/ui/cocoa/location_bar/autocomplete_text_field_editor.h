// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/url_drop_target.h"

@class AutocompleteTextField;
class AutocompleteTextFieldObserver;


@interface AutocompleteTextFieldEditor : NSTextView<URLDropTarget> {
  
  
  
  
  base::scoped_nsobject<URLDropTargetHandler> dropHandler_;

  base::scoped_nsobject<NSCharacterSet> forbiddenCharacters_;

  
  
  
  
  
  
  BOOL interpretingKeyEvents_;

  
  BOOL textChangedByKeyEvents_;
}

- (AutocompleteTextField*)delegate;
- (void)setDelegate:(AutocompleteTextField*)delegate;

- (void)setAttributedString:(NSAttributedString*)aString;

@end

@interface AutocompleteTextFieldEditor(PrivateTestMethods)
- (AutocompleteTextFieldObserver*)observer;
- (void)pasteAndGo:sender;
- (void)showURL:sender;
@end
