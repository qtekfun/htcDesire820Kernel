// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_VIEW_MANAGER_OBSERVER_H_
#define CHROME_BROWSER_PRINTING_PRINT_VIEW_MANAGER_OBSERVER_H_

namespace printing {

class PrintViewManagerObserver {
 public:
  
  virtual void OnPrintDialogShown() = 0;

 protected:
  virtual ~PrintViewManagerObserver() {}
};

}  

#endif  
