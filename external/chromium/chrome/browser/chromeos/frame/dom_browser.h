// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FRAME_DOM_BROWSER_H_
#define CHROME_BROWSER_CHROMEOS_FRAME_DOM_BROWSER_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/ui/browser.h"

class Profile;

namespace chromeos {

class DOMBrowser : public Browser {
 public:
  explicit DOMBrowser(Profile* profile);
  virtual ~DOMBrowser();

  
  
  static DOMBrowser* CreateForDOM(Profile* profile);

 protected:
  
  virtual BrowserWindow* CreateBrowserWindow();

 private:
  DISALLOW_COPY_AND_ASSIGN(DOMBrowser);
};

}  

#endif  
