// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STATUS_ICONS_STATUS_TRAY_H_
#define CHROME_BROWSER_STATUS_ICONS_STATUS_TRAY_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"

class StatusIcon;

class StatusTray {
 public:
  
  
  
  static StatusTray* Create();

  virtual ~StatusTray();

  
  
  StatusIcon* CreateStatusIcon();

  
  void RemoveStatusIcon(StatusIcon* icon);

 protected:
  StatusTray();
  
  virtual StatusIcon* CreatePlatformStatusIcon() = 0;

  
  
  void RemoveAllIcons();

  typedef std::vector<StatusIcon*> StatusIconList;
  
  const StatusIconList& status_icons() { return status_icons_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(StatusTrayTest, CreateRemove);

  
  StatusIconList status_icons_;

  DISALLOW_COPY_AND_ASSIGN(StatusTray);
};

#endif  
