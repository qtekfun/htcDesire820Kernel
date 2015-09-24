// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_MAIN_MENU_ITEM_H_
#define CHROME_BROWSER_UI_COCOA_MAIN_MENU_ITEM_H_

class MainMenuItem {
 public:
  
  
  virtual void ResetMenu() = 0;

  
  virtual void BuildMenu() = 0;

 protected:
  virtual ~MainMenuItem() {}
};

#endif  
