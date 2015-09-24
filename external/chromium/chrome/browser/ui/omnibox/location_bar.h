// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_OMNIBOX_LOCATION_BAR_H_
#define CHROME_BROWSER_UI_OMNIBOX_LOCATION_BAR_H_
#pragma once

#include <string>

#include "base/string16.h"
#include "chrome/browser/first_run/first_run.h"
#include "chrome/browser/instant/instant_delegate.h"
#include "content/common/page_transition_types.h"
#include "webkit/glue/window_open_disposition.h"

class AutocompleteEditView;
class ExtensionAction;
class InstantController;
class LocationBarTesting;
class TabContents;
class TabContentsWrapper;

class LocationBar {
 public:
  
  virtual void ShowFirstRunBubble(FirstRun::BubbleType bubble_type) = 0;

  
  
  virtual void SetSuggestedText(const string16& text,
                                InstantCompleteBehavior behavior) = 0;

  
  virtual std::wstring GetInputString() const = 0;

  
  
  virtual WindowOpenDisposition GetWindowOpenDisposition() const = 0;

  
  
  virtual PageTransition::Type GetPageTransition() const = 0;

  
  virtual void AcceptInput() = 0;

  
  virtual void FocusLocation(bool select_all) = 0;

  
  
  virtual void FocusSearch() = 0;

  
  virtual void UpdateContentSettingsIcons() = 0;

  
  virtual void UpdatePageActions() = 0;

  
  
  virtual void InvalidatePageActions() = 0;

  
  
  virtual void SaveStateToContents(TabContents* contents) = 0;

  
  virtual void Revert() = 0;

  
  virtual const AutocompleteEditView* location_entry() const = 0;
  virtual AutocompleteEditView* location_entry() = 0;

  
  virtual LocationBarTesting* GetLocationBarForTesting() = 0;

 protected:
  virtual ~LocationBar() {}
};

class LocationBarTesting {
 public:
  
  virtual int PageActionCount() = 0;

  
  virtual int PageActionVisibleCount() = 0;

  
  virtual ExtensionAction* GetPageAction(size_t index) = 0;

  
  virtual ExtensionAction* GetVisiblePageAction(size_t index) = 0;

  
  virtual void TestPageActionPressed(size_t index) = 0;

 protected:
  virtual ~LocationBarTesting() {}
};

#endif  
