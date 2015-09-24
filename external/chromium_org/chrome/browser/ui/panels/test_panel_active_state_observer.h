// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_TEST_PANEL_ACTIVE_STATE_OBSERVER_H_
#define CHROME_BROWSER_UI_PANELS_TEST_PANEL_ACTIVE_STATE_OBSERVER_H_

#include "chrome/browser/ui/panels/test_panel_notification_observer.h"

class Panel;

class PanelActiveStateObserver : public TestPanelNotificationObserver {
 public:
  
  
  
  PanelActiveStateObserver(Panel* panel, bool expect_active);
  virtual ~PanelActiveStateObserver();

 private:
  
  virtual bool AtExpectedState() OVERRIDE;

  Panel* panel_;
  bool expect_active_;

  DISALLOW_COPY_AND_ASSIGN(PanelActiveStateObserver);
};

#endif  
