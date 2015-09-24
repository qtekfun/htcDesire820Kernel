// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_POPUP_MODEL_OBSERVER_H_
#define CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_POPUP_MODEL_OBSERVER_H_

class OmniboxPopupModelObserver {
 public:
  
  virtual void OnOmniboxPopupShownOrHidden() = 0;
};

#endif  
