// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_L10N_TIME_FORMAT_H_
#define UI_BASE_L10N_TIME_FORMAT_H_

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "ui/base/ui_export.h"

namespace base {
class Time;
class TimeDelta;
}

namespace ui {

class UI_EXPORT TimeFormat {
 public:
  
  
  
  

  
  static base::string16 TimeElapsed(const base::TimeDelta& delta);

  
  static base::string16 TimeRemaining(const base::TimeDelta& delta);

  
  
  
  static base::string16 TimeRemainingLong(const base::TimeDelta& delta);

  
  static base::string16 TimeRemainingShort(const base::TimeDelta& delta);

  
  static base::string16 TimeDurationLong(const base::TimeDelta& delta);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static base::string16 RelativeDate(const base::Time& time,
                                     const base::Time* optional_midnight_today);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(TimeFormat);
};

}  

#endif  
