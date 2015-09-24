// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WIN_HWND_SUBCLASS_H_
#define UI_BASE_WIN_HWND_SUBCLASS_H_

#include <vector>
#include <windows.h>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/ui_export.h"
#include "ui/base/view_prop.h"

namespace ui {

class UI_EXPORT HWNDMessageFilter {
 public:
  virtual ~HWNDMessageFilter();

  
  
  
  
  
  
  
  
  virtual bool FilterMessage(HWND hwnd,
                             UINT message,
                             WPARAM w_param,
                             LPARAM l_param,
                             LRESULT* l_result) = 0;
};

class UI_EXPORT HWNDSubclass {
 public:
  ~HWNDSubclass();

  
  
  
  static void AddFilterToTarget(HWND target, HWNDMessageFilter* filter);

  
  static void RemoveFilterFromAllTargets(HWNDMessageFilter* filter);

  
  
  static HWNDSubclass* GetHwndSubclassForTarget(HWND target);

  
  
  
  void AddFilter(HWNDMessageFilter* filter);

  
  void RemoveFilter(HWNDMessageFilter* filter);

  LRESULT OnWndProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param);

 private:
  class HWNDSubclassFactory;
  friend class HWNDSubclassFactory;

  explicit HWNDSubclass(HWND target);

  HWND target_;
  std::vector<HWNDMessageFilter*> filters_;
  WNDPROC original_wnd_proc_;
  ui::ViewProp prop_;

  DISALLOW_COPY_AND_ASSIGN(HWNDSubclass);
};

}  

#endif  
