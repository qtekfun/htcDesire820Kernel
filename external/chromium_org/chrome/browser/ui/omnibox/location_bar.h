// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_OMNIBOX_LOCATION_BAR_H_
#define CHROME_BROWSER_UI_OMNIBOX_LOCATION_BAR_H_

#include <string>

#include "base/strings/string16.h"
#include "content/public/common/page_transition_types.h"
#include "ui/base/window_open_disposition.h"
#include "url/gurl.h"

class ExtensionAction;
class LocationBarTesting;
class OmniboxView;

namespace content {
class WebContents;
}

class LocationBar {
 public:
  
  virtual void ShowFirstRunBubble() = 0;

  
  virtual GURL GetDestinationURL() const = 0;
  virtual WindowOpenDisposition GetWindowOpenDisposition() const = 0;
  virtual content::PageTransition GetPageTransition() const = 0;

  
  virtual void AcceptInput() = 0;

  
  virtual void FocusLocation(bool select_all) = 0;

  
  
  virtual void FocusSearch() = 0;

  
  virtual void UpdateContentSettingsIcons() = 0;

  
  virtual void UpdateManagePasswordsIconAndBubble() = 0;

  
  virtual void UpdatePageActions() = 0;

  
  
  virtual void InvalidatePageActions() = 0;

  
  virtual void UpdateOpenPDFInReaderPrompt() = 0;

  
  
  
  virtual void UpdateGeneratedCreditCardView() = 0;

  
  
  virtual void SaveStateToContents(content::WebContents* contents) = 0;

  
  virtual void Revert() = 0;

  virtual const OmniboxView* GetOmniboxView() const = 0;
  virtual OmniboxView* GetOmniboxView() = 0;

  
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

  
  virtual bool GetBookmarkStarVisibility() = 0;

 protected:
  virtual ~LocationBarTesting() {}
};

#endif  
