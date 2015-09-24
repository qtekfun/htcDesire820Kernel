// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SCOPED_TABBED_BROWSER_DISPLAYER_H_
#define CHROME_BROWSER_UI_SCOPED_TABBED_BROWSER_DISPLAYER_H_

#include "base/basictypes.h"
#include "chrome/browser/ui/host_desktop.h"

class Browser;
class Profile;

namespace chrome {

class ScopedTabbedBrowserDisplayer {
 public:
  ScopedTabbedBrowserDisplayer(Profile* profile, HostDesktopType type);
  ~ScopedTabbedBrowserDisplayer();

  Browser* browser() {
    return browser_;
  }

 private:
  Browser* browser_;

  DISALLOW_COPY_AND_ASSIGN(ScopedTabbedBrowserDisplayer);
};

}  

#endif  
