// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_CONTENT_BROWSER_TAB_CONTENTS_TAB_CONTENTS_OBSERVER_H_
#define ANDROID_CONTENT_BROWSER_TAB_CONTENTS_TAB_CONTENTS_OBSERVER_H_


#include "content/browser/tab_contents/tab_contents.h"

class TabContentsObserver {
 public:

 protected:
  TabContentsObserver(TabContents* tab_contents) {
    tab_contents_ = tab_contents;
  }

  virtual ~TabContentsObserver() {}

  TabContents* tab_contents() { return tab_contents_; }

 private:
  friend class TabContents;

  TabContents* tab_contents_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsObserver);
};

#endif  
