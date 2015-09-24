// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_I18N_TIME_FORMATTING_H_
#define BASE_I18N_TIME_FORMATTING_H_
#pragma once

#include "base/string16.h"

namespace base {

class Time;

enum HourClockType {
  k12HourClock,  
  k24HourClock,  
};

string16 TimeFormatTimeOfDay(const Time& time);

string16 TimeFormatTimeOfDayWithHourClockType(const Time& time,
                                              HourClockType type);

string16 TimeFormatShortDate(const Time& time);

string16 TimeFormatShortDateNumeric(const Time& time);

string16 TimeFormatShortDateAndTime(const Time& time);

string16 TimeFormatFriendlyDateAndTime(const Time& time);

string16 TimeFormatFriendlyDate(const Time& time);

HourClockType GetHourClockType();

}  

#endif  
