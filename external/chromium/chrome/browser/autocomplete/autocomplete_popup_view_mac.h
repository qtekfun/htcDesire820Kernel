// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_POPUP_VIEW_MAC_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_POPUP_VIEW_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/autocomplete/autocomplete.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/autocomplete/autocomplete_popup_view.h"
#import "chrome/browser/ui/cocoa/location_bar/instant_opt_in_controller.h"
#include "ui/gfx/font.h"
#include "webkit/glue/window_open_disposition.h"


class AutocompleteEditModel;
class AutocompleteEditViewMac;
@class AutocompleteMatrix;
class AutocompletePopupModel;
@class InstantOptInController;
@class NSImage;
class Profile;


class AutocompletePopupViewMac : public AutocompletePopupView,
                                 public InstantOptInControllerDelegate {
 public:
  AutocompletePopupViewMac(AutocompleteEditViewMac* edit_view,
                           AutocompleteEditModel* edit_model,
                           Profile* profile,
                           NSTextField* field);
  virtual ~AutocompletePopupViewMac();

  
  virtual void UserPressedOptIn(bool opt_in);

  
  virtual bool IsOpen() const;
  virtual void InvalidateLine(size_t line) {
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
  }
  virtual void UpdatePopupAppearance();

  virtual gfx::Rect GetTargetBounds();

  
  void SetSelectedLine(size_t line);

  
  
  virtual void PaintUpdatesNow();

  virtual void OnDragCanceled() {}

  
  
  
  
  void OpenURLForRow(int row, bool force_background);

  
  
  
  static NSAttributedString* MatchText(const AutocompleteMatch& match,
                                gfx::Font& font,
                                float cellWidth);

  
  
  
  static NSMutableAttributedString* DecorateMatchedString(
      const string16 &matchString,
      const AutocompleteMatch::ACMatchClassifications &classifications,
      NSColor* textColor, NSColor* dimTextColor, gfx::Font& font);

  
  
  
  
  
  
  static NSMutableAttributedString* ElideString(
      NSMutableAttributedString* aString,
      const string16 originalString,
      const gfx::Font& font,
      const float cellWidth);

 private:
  
  AutocompleteMatrix* GetAutocompleteMatrix();

  
  void CreatePopupIfNeeded();

  
  
  
  
  
  
  void PositionPopup(const CGFloat matrixHeight);

  
  NSImage* ImageForMatch(const AutocompleteMatch& match);

  
  bool ShouldShowInstantOptIn();

  scoped_ptr<AutocompletePopupModel> model_;
  AutocompleteEditViewMac* edit_view_;
  NSTextField* field_;  

  
  scoped_nsobject<NSWindow> popup_;
  scoped_nsobject<InstantOptInController> opt_in_controller_;
  NSRect targetPopupFrame_;

  DISALLOW_COPY_AND_ASSIGN(AutocompletePopupViewMac);
};

#endif  
