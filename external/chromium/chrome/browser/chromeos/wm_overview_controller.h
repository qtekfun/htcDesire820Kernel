// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_WM_OVERVIEW_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_WM_OVERVIEW_CONTROLLER_H_
#pragma once

#include <vector>

#include "base/memory/linked_ptr.h"
#include "base/memory/singleton.h"
#include "base/timer.h"
#include "chrome/browser/chromeos/wm_message_listener.h"
#include "chrome/browser/ui/browser_list.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/rect.h"

namespace views {
class Widget;
}

class Browser;
class RenderWidgetHost;

namespace chromeos {

class BrowserListener;


class WmOverviewController : public BrowserList::Observer,
                             public WmMessageListener::Observer,
                             public NotificationObserver {
 public:
  
  
  enum LayoutMode {
    
    
    
    ACTIVE_MODE,

    
    
    
    
    OVERVIEW_MODE,
  };

  
  static WmOverviewController* GetInstance();

  
  void OnBrowserAdded(const Browser* browser) {}
  void OnBrowserRemoved(const Browser* browser);
  

  
  
  void ProcessWmMessage(const WmIpc::Message& message,
                        GdkWindow* window);
  

  
  void Observe(NotificationType type,
               const NotificationSource& source,
               const NotificationDetails& details);
  

  
  const gfx::Rect& monitor_bounds() const { return monitor_bounds_; }

  
  void UpdateSnapshots();

  
  
  
  void StartDelayTimer();

  LayoutMode layout_mode() const { return layout_mode_; }

 private:
  friend struct DefaultSingletonTraits<WmOverviewController>;

  
  WmOverviewController();
  ~WmOverviewController();

  
  
  void RestoreTabSelections();

  
  
  void SaveTabSelections();

  
  void Show();

  
  
  
  void Hide(bool cancelled);

  
  
  void AddAllBrowsers();

  
  
  
  void SnapshotImageChanged(RenderWidgetHost* renderer);

  
  NotificationRegistrar registrar_;

  
  typedef std::vector<linked_ptr<BrowserListener> > BrowserListenerVector;
  BrowserListenerVector listeners_;

  
  
  gfx::Rect monitor_bounds_;

  
  base::OneShotTimer<WmOverviewController> delay_timer_;

  
  LayoutMode layout_mode_;

  
  
  bool updating_snapshots_;

  
  
  
  int browser_listener_index_;  
  int tab_contents_index_;      

  DISALLOW_COPY_AND_ASSIGN(WmOverviewController);
};

}  

#endif  
