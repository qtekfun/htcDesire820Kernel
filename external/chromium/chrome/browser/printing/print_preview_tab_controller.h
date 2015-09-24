// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_PREVIEW_TAB_CONTROLLER_H_

#define CHROME_BROWSER_PRINTING_PRINT_PREVIEW_TAB_CONTROLLER_H_
#pragma once

#include <map>

#include "base/memory/ref_counted.h"
#include "chrome/browser/sessions/session_id.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Browser;
class TabContents;

namespace printing {

class PrintPreviewTabController
    : public base::RefCounted<PrintPreviewTabController>,
      public NotificationObserver {
 public:
  PrintPreviewTabController();

  virtual ~PrintPreviewTabController();

  static PrintPreviewTabController* GetInstance();

  
  
  static void PrintPreview(TabContents* initiator_tab);

  
  
  TabContents* GetOrCreatePreviewTab(TabContents* initiator_tab);

  
  
  
  TabContents* GetPrintPreviewForTab(TabContents* tab) const;

  
  
  TabContents* GetInitiatorTab(TabContents* preview_tab);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  static bool IsPrintPreviewTab(TabContents* tab);

 private:
  friend class base::RefCounted<PrintPreviewTabController>;

  
  
  
  typedef std::map<TabContents*, TabContents*> PrintPreviewTabMap;

  
  TabContents* CreatePrintPreviewTab(TabContents* initiator_tab);

  
  void AddObservers(TabContents* tab);
  void RemoveObservers(TabContents* tab);

  PrintPreviewTabMap preview_tab_map_;

  
  NotificationRegistrar registrar_;

  
  
  bool waiting_for_new_preview_page_;

  DISALLOW_COPY_AND_ASSIGN(PrintPreviewTabController);
};

}  

#endif  
