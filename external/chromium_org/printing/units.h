// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_UNITS_H_
#define PRINTING_UNITS_H_

#include "printing/printing_export.h"

namespace printing {

const int kHundrethsMMPerInch = 2540;

const int kPointsPerInch = 72;

const int kPixelsPerInch = 96;

const int kDefaultPdfDpi = 300;

const float kLetterWidthInch = 8.5f;
const float kLetterHeightInch = 11.0f;

const float kLegalWidthInch = 8.5f;
const float kLegalHeightInch = 14.0f;

const float kA4WidthInch = 8.27f;
const float kA4HeightInch = 11.69f;

const float kA3WidthInch = 11.69f;
const float kA3HeightInch = 16.54f;

PRINTING_EXPORT int ConvertUnit(int value, int old_unit, int new_unit);

PRINTING_EXPORT double ConvertUnitDouble(double value, double old_unit,
                                         double new_unit);

PRINTING_EXPORT int ConvertMilliInchToHundredThousanthMeter(int milli_inch);

PRINTING_EXPORT int ConvertHundredThousanthMeterToMilliInch(int cmm);

PRINTING_EXPORT int ConvertPixelsToPoint(int pixels);

PRINTING_EXPORT double ConvertPixelsToPointDouble(double pixels);

double ConvertPointsToPixelDouble(double points);

}  

#endif  
