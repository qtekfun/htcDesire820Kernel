// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_TIME_FORMAT_H__
#define CHROME_COMMON_TIME_FORMAT_H__
#pragma once


#include "base/string16.h"

#include "unicode/smpdtfmt.h"

namespace base {
class Time;
class TimeDelta;
}

class TimeFormat {
 public:
  
  
  
  

  
  static string16 TimeElapsed(const base::TimeDelta& delta);

  
  static string16 TimeRemaining(const base::TimeDelta& delta);

  
  static string16 TimeRemainingShort(const base::TimeDelta& delta);

  
  
  
  
  
  
  
  
  
  
  
  
  
  static string16 RelativeDate(const base::Time& time,
                               const base::Time* optional_midnight_today);
};

#endif  
