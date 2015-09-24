// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_TEST_PANEL_COLLECTION_SQUEEZE_OBSERVER_H_
#define CHROME_BROWSER_UI_PANELS_TEST_PANEL_COLLECTION_SQUEEZE_OBSERVER_H_

#include "chrome/browser/ui/panels/test_panel_notification_observer.h"

class DockedPanelCollection;
class Panel;

class PanelCollectionSqueezeObserver : public TestPanelNotificationObserver {
 public:
  
  
  
  
  PanelCollectionSqueezeObserver(DockedPanelCollection* collection,
                                 Panel* active_panel);
  virtual ~PanelCollectionSqueezeObserver();

 private:
  
  virtual bool AtExpectedState() OVERRIDE;

  bool IsSqueezed(Panel* panel);

  DockedPanelCollection* panel_collection_;
  Panel* active_panel_;

  DISALLOW_COPY_AND_ASSIGN(PanelCollectionSqueezeObserver);
};

#endif  
