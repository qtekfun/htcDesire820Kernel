// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_I18N_TIME_FORMATTING_H_
#define BASE_I18N_TIME_FORMATTING_H_

#include "base/i18n/base_i18n_export.h"
#include "base/strings/string16.h"

namespace base {

class Time;

enum HourClockType {
  k12HourClock,  
  k24HourClock,  
};

enum AmPmClockType {
  kDropAmPm,  
  kKeepAmPm,  
};

BASE_I18N_EXPORT string16 TimeFormatTimeOfDay(const Time& time);

BASE_I18N_EXPORT string16 TimeFormatTimeOfDayWithHourClockType(
    const Time& time,
    HourClockType type,
    AmPmClockType ampm);

BASE_I18N_EXPORT string16 TimeFormatShortDate(const Time& time);

BASE_I18N_EXPORT string16 TimeFormatShortDateNumeric(const Time& time);

BASE_I18N_EXPORT string16 TimeFormatShortDateAndTime(const Time& time);

BASE_I18N_EXPORT string16 TimeFormatFriendlyDateAndTime(const Time& time);

BASE_I18N_EXPORT string16 TimeFormatFriendlyDate(const Time& time);

BASE_I18N_EXPORT HourClockType GetHourClockType();

}  

#endif  
