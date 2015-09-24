// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_INTERSTITIAL_PAGE_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_INTERSTITIAL_PAGE_DELEGATE_H_

#include <string>

#include "content/common/content_export.h"

namespace content {

class NavigationEntry;
struct RendererPreferences;

class InterstitialPageDelegate {
 public:
  virtual ~InterstitialPageDelegate() {}

  
  virtual std::string GetHTMLContents() = 0;

  
  
  
  virtual void OnProceed() {}
  virtual void OnDontProceed() {}

  
  virtual void CommandReceived(const std::string& command) {}

  
  
  
  
  
  virtual void OverrideEntry(content::NavigationEntry* entry) {}

  
  
  virtual void OverrideRendererPrefs(content::RendererPreferences* prefs) {}
};

}  

#endif  
