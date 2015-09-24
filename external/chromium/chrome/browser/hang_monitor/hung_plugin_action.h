// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HANG_MONITOR_HUNG_PLUGIN_ACTION_H__
#define CHROME_BROWSER_HANG_MONITOR_HUNG_PLUGIN_ACTION_H__
#pragma once

#include "chrome/browser/hang_monitor/hung_window_detector.h"
class HungPluginAction : public HungWindowDetector::HungWindowNotification {
 public:
  HungPluginAction();
  ~HungPluginAction();
  
  virtual bool OnHungWindowDetected(HWND hung_window,
                                    HWND top_level_window,
                                    ActionOnHungWindow* action);

 protected:
  void OnWindowResponsive(HWND window);

  
  static void CALLBACK HungWindowResponseCallback(HWND target_window,
                                                  UINT message,
                                                  ULONG_PTR data,
                                                  LRESULT result);

  static BOOL CALLBACK DismissMessageBox(HWND window, LPARAM ignore);

 protected:
  bool GetPluginName(HWND plugin_window,
                     DWORD browser_process_id,
                     std::wstring *plugin_name);
  
  HWND current_hung_plugin_window_;
};

#endif  
