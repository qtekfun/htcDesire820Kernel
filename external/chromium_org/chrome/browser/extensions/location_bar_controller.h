// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_LOCATION_BAR_CONTROLLER_H_
#define CHROME_BROWSER_EXTENSIONS_LOCATION_BAR_CONTROLLER_H_

#include <set>
#include <string>
#include <vector>

class ExtensionAction;

namespace extensions {

class LocationBarController {
 public:
  
  enum Action {
    ACTION_NONE,
    ACTION_SHOW_POPUP,
    ACTION_SHOW_CONTEXT_MENU,
    ACTION_SHOW_SCRIPT_POPUP,
  };

  virtual ~LocationBarController() {}

  
  virtual std::vector<ExtensionAction*> GetCurrentActions() const = 0;

  
  
  virtual void GetAttentionFor(const std::string& extension_id) = 0;

  
  
  
  
  virtual Action OnClicked(const std::string& extension_id,
                           int mouse_button) = 0;

  
  virtual void NotifyChange() = 0;
};

}  

#endif  
