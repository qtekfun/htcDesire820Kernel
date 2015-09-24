// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TAB_CONTENTS_TAB_CONTENTS_ITERATOR_H_
#define CHROME_BROWSER_UI_TAB_CONTENTS_TAB_CONTENTS_ITERATOR_H_

#include <set>

#include "base/basictypes.h"
#include "chrome/browser/ui/browser_iterator.h"

namespace content {
class WebContents;
}

class TabContentsIterator {
 public:
  TabContentsIterator();

  
  bool done() const { return cur_ == NULL; }

  
  
  Browser* browser() const {
    if (!browser_iterator_.done())
      return *browser_iterator_;
    return NULL;
  }

  
  content::WebContents* operator->() const {
    return cur_;
  }
  content::WebContents* operator*() const {
    return cur_;
  }

  
  
  
  void Next();

 private:
  
  int web_view_index_;

  
  
  
  content::WebContents* cur_;

  
  chrome::BrowserIterator browser_iterator_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsIterator);
};

#endif  
