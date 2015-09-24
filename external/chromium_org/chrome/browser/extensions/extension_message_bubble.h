// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_MESSAGE_BUBBLE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_MESSAGE_BUBBLE_H_

#include "base/bind.h"

class Browser;

namespace extensions {

class ExtensionMessageBubble {
 public:
  
  
  virtual void OnActionButtonClicked(const base::Closure& callback) = 0;

  
  virtual void OnDismissButtonClicked(const base::Closure& callback) = 0;

  
  virtual void OnLinkClicked(const base::Closure& callback) = 0;

  
  virtual void Show() = 0;
};

}  

#endif  
