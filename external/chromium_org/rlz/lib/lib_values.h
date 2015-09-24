// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RLZ_LIB_LIB_VALUES_H_
#define RLZ_LIB_LIB_VALUES_H_

#include "base/basictypes.h"
#include "rlz/lib/rlz_enums.h"

namespace rlz_lib {

extern const char kEventsCgiVariable[];
extern const char kStatefulEventsCgiVariable[];
extern const char kEventsCgiSeparator;

extern const char kDccCgiVariable[];
extern const char kProtocolCgiArgument[];

extern const char kProductSignatureCgiVariable[];
extern const char kProductBrandCgiVariable[];
extern const char kProductLanguageCgiVariable[];
extern const char kProductIdCgiVariable[];

extern const char kRlzCgiVariable[];
extern const char kRlzCgiSeparator[];
extern const char kRlzCgiIndicator[];

extern const char kRlsCgiVariable[];
extern const char kMachineIdCgiVariable[];
extern const char kSetDccResponseVariable[];


extern const char kFinancialPingPath[];
extern const char kFinancialServer[];

extern const int kFinancialPort;

extern const int64 kEventsPingInterval;
extern const int64 kNoEventsPingInterval;

extern const char kFinancialPingUserAgent[];
extern const char* kFinancialPingResponseObjects[];

const char* GetAccessPointName(AccessPoint point);
bool GetAccessPointFromName(const char* name, AccessPoint* point);

const char* GetEventName(Event event);
bool GetEventFromName(const char* name, Event* event);

const char* GetProductName(Product product);

}  

#endif  
