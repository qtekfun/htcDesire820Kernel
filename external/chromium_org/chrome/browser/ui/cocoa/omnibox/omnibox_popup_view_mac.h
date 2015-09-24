// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_OMNIBOX_OMNIBOX_POPUP_VIEW_MAC_H_
#define CHROME_BROWSER_UI_COCOA_OMNIBOX_OMNIBOX_POPUP_VIEW_MAC_H_

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#import "chrome/browser/ui/cocoa/omnibox/omnibox_popup_matrix.h"
#include "chrome/browser/ui/omnibox/omnibox_popup_view.h"
#include "ui/gfx/font.h"

class AutocompleteResult;
class OmniboxEditModel;
class OmniboxPopupModel;
class OmniboxView;

class OmniboxPopupViewMac : public OmniboxPopupView,
                            public OmniboxPopupMatrixDelegate {
 public:
  OmniboxPopupViewMac(OmniboxView* omnibox_view,
                      OmniboxEditModel* edit_model,
                      NSTextField* field);
  virtual ~OmniboxPopupViewMac();

  
  virtual bool IsOpen() const OVERRIDE;
  virtual void InvalidateLine(size_t line) OVERRIDE {}
  virtual void UpdatePopupAppearance() OVERRIDE;
  virtual gfx::Rect GetTargetBounds() OVERRIDE;
  
  
  virtual void PaintUpdatesNow() OVERRIDE;
  virtual void OnDragCanceled() OVERRIDE {}

  
  virtual void OnMatrixRowSelected(OmniboxPopupMatrix* matrix,
                                   size_t row) OVERRIDE;
  virtual void OnMatrixRowClicked(OmniboxPopupMatrix* matrix,
                                  size_t row) OVERRIDE;
  virtual void OnMatrixRowMiddleClicked(OmniboxPopupMatrix* matrix,
                                        size_t row) OVERRIDE;

  OmniboxPopupMatrix* matrix() { return matrix_; }

  
  
  
  static NSAttributedString* MatchText(const AutocompleteMatch& match,
                                       gfx::Font& font,
                                       float cell_width);

  
  
  static NSMutableAttributedString* DecorateMatchedString(
      const base::string16& match_string,
      const AutocompleteMatch::ACMatchClassifications& classifications,
      NSColor* text_color,
      NSColor* dim_text_color,
      gfx::Font& font);

  
  
  
  
  
  
  static NSMutableAttributedString* ElideString(
      NSMutableAttributedString* a_string,
      const base::string16& original_string,
      const gfx::Font& font,
      const float cell_width);

 protected:
  
  
  virtual const AutocompleteResult& GetResult() const;

 private:
  
  void CreatePopupIfNeeded();

  
  
  
  
  
  
  void PositionPopup(const CGFloat matrixHeight);

  
  NSImage* ImageForMatch(const AutocompleteMatch& match);

  
  void OpenURLForRow(size_t row, WindowOpenDisposition disposition);

  OmniboxView* omnibox_view_;
  scoped_ptr<OmniboxPopupModel> model_;
  NSTextField* field_;  

  
  base::scoped_nsobject<NSWindow> popup_;
  NSRect target_popup_frame_;

  base::scoped_nsobject<OmniboxPopupMatrix> matrix_;
  base::scoped_nsobject<NSView> top_separator_view_;
  base::scoped_nsobject<NSView> bottom_separator_view_;
  base::scoped_nsobject<NSBox> background_view_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxPopupViewMac);
};

#endif  
