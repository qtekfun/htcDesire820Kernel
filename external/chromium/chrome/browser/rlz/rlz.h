// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RLZ_RLZ_H_
#define CHROME_BROWSER_RLZ_RLZ_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_WIN)

#include <string>

#include "base/basictypes.h"
#include "rlz/win/lib/rlz_lib.h"


class RLZTracker {

 public:
  
  
  
  
  
  
  
  static bool InitRlzDelayed(bool first_run, int delay);

  
  
  
  static bool RecordProductEvent(rlz_lib::Product product,
                                 rlz_lib::AccessPoint point,
                                 rlz_lib::Event event_id);

  
  
  
  static bool GetAccessPointRlz(rlz_lib::AccessPoint point, std::wstring* rlz);

  
  
  static bool ClearAllProductEvents(rlz_lib::Product product);

  
  static void CleanupRlz();

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(RLZTracker);
};

#endif  

#endif  
