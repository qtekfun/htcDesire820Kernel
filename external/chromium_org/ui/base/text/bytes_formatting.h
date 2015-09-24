// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_TEXT_BYTES_FORMATTING_H_
#define UI_BASE_TEXT_BYTES_FORMATTING_H_

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "ui/base/ui_export.h"

namespace ui {


UI_EXPORT base::string16 FormatBytes(int64 bytes);

UI_EXPORT base::string16 FormatSpeed(int64 bytes);


enum DataUnits {
  DATA_UNITS_BYTE = 0,
  DATA_UNITS_KIBIBYTE,
  DATA_UNITS_MEBIBYTE,
  DATA_UNITS_GIBIBYTE,
  DATA_UNITS_TEBIBYTE,
  DATA_UNITS_PEBIBYTE
};

UI_EXPORT DataUnits GetByteDisplayUnits(int64 bytes);

UI_EXPORT base::string16 FormatBytesWithUnits(int64 bytes,
                                              DataUnits units,
                                              bool show_units);

base::string16 FormatSpeedWithUnits(int64 bytes,
                                    DataUnits units,
                                    bool show_units);

}  

#endif  
