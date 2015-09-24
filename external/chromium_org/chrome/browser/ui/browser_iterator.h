// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_ITERATOR_H_
#define CHROME_BROWSER_UI_BROWSER_ITERATOR_H_

#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/host_desktop.h"

class Browser;

namespace chrome {

class BrowserIterator {
 public:
  BrowserIterator();
  ~BrowserIterator();

  
  bool done() const {
    
    
    
    return current_iterator_ == current_browser_list_->end();
  }

  
  Browser* operator->() const {
    return *current_iterator_;
  }
  Browser* operator*() const {
    return *current_iterator_;
  }

  
  void Next();

 private:
  
  
  
  void NextBrowserListIfAtEnd();

  
  
  BrowserList* current_browser_list_;

  
  BrowserList::const_iterator current_iterator_;

  
  
  HostDesktopType next_desktop_type_;

  DISALLOW_COPY_AND_ASSIGN(BrowserIterator);
};

}  

#endif  
