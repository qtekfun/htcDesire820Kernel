// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HANG_MONITOR_HUNG_WINDOW_DETECTOR_H__
#define CHROME_BROWSER_HANG_MONITOR_HUNG_WINDOW_DETECTOR_H__

#include "base/synchronization/lock.h"
#include "chrome/common/worker_thread_ticker.h"

class HungWindowDetector : public WorkerThreadTicker::Callback {
 public:
  
  static const wchar_t kHungChildWindowTimeout[];
  
  
  class HungWindowNotification  {
   public:
    enum ActionOnHungWindow {
      HUNG_WINDOW_IGNORE,
      HUNG_WINDOW_TERMINATE_PROCESS,
    };

    
    
    
    virtual bool OnHungWindowDetected(HWND hung_window, HWND top_level_window,
                                      ActionOnHungWindow* action) = 0;
  };

  
  
  
  
  
  explicit HungWindowDetector(HungWindowNotification* notification);
  ~HungWindowDetector();

  
  
  
  
  
  
  
  
  bool Initialize(HWND top_level_window,
                  int message_response_timeout);

  
  virtual void OnTick();

 private:
  
  
  bool CheckChildWindow(HWND child_window);

  static BOOL CALLBACK ChildWndEnumProc(HWND child_window, LPARAM param);

  
  
  
  HungWindowNotification* notification_;
  HWND top_level_window_;

  
  int message_response_timeout_;
  base::Lock hang_detection_lock_;
  
  bool enumerating_;

  DISALLOW_COPY_AND_ASSIGN(HungWindowDetector);
};


#endif  
