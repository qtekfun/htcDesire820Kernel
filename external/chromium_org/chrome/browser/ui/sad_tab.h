// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SAD_TAB_H_
#define CHROME_BROWSER_UI_SAD_TAB_H_

#include "base/process/kill.h"
#include "chrome/browser/ui/sad_tab_types.h"

namespace content {
class WebContents;
}

namespace chrome {

class SadTab {
 public:
  
  static SadTab* Create(content::WebContents* web_contents, SadTabKind kind);

  
  static bool ShouldShow(base::TerminationStatus status);

  virtual ~SadTab() {}

  
  virtual void Show() = 0;

  
  virtual void Close() = 0;
};

}  

#endif  
