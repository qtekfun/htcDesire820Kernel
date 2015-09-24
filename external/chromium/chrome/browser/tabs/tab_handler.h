// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TABS_TAB_HANDLER_H_
#define CHROME_BROWSER_TABS_TAB_HANDLER_H_
#pragma once

class Browser;
class Profile;
class TabStripModel;

class TabHandlerDelegate {
 public:
  virtual Profile* GetProfile() const = 0;

  
  virtual Browser* AsBrowser() = 0;
};

class TabHandler {
 public:
  virtual ~TabHandler() {}

  
  
  static TabHandler* CreateTabHandler(TabHandlerDelegate* delegate);

  
  virtual TabStripModel* GetTabStripModel() const = 0;
};

#endif  
