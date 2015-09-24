// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_INFOBARS_INTERNAL_DISPLACED_WINDOW_MANAGER_H_
#define CHROME_FRAME_INFOBARS_INTERNAL_DISPLACED_WINDOW_MANAGER_H_

#include <atlbase.h>
#include <atlcrack.h>
#include <atlwin.h>

#include "base/basictypes.h"
#include "chrome_frame/infobars/internal/subclassing_window_with_delegate.h"

class DisplacedWindowManager
    : public SubclassingWindowWithDelegate<DisplacedWindowManager> {
 public:
  DisplacedWindowManager();

  
  
  
  void UpdateLayout();

  BEGIN_MSG_MAP_EX(DisplacedWindowManager)
    MSG_WM_NCCALCSIZE(OnNcCalcSize)
    CHAIN_MSG_MAP(SubclassingWindowWithDelegate<DisplacedWindowManager>)
  END_MSG_MAP()

 private:
  
  
  
  LRESULT OnNcCalcSize(BOOL calc_valid_rects, LPARAM lparam);

  DISALLOW_COPY_AND_ASSIGN(DisplacedWindowManager);
};  

#endif  
