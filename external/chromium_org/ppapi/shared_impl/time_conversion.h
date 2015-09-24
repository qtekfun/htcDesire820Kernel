// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_TIME_CONVERSION_H_
#define PPAPI_SHARED_IMPL_TIME_CONVERSION_H_

#include "base/time/time.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

PPAPI_SHARED_EXPORT PP_Time TimeToPPTime(base::Time t);
PPAPI_SHARED_EXPORT base::Time PPTimeToTime(PP_Time t);

PPAPI_SHARED_EXPORT PP_TimeTicks TimeTicksToPPTimeTicks(base::TimeTicks t);

PPAPI_SHARED_EXPORT PP_TimeTicks EventTimeToPPTimeTicks(double event_time);
PPAPI_SHARED_EXPORT double PPTimeTicksToEventTime(PP_TimeTicks t);

PPAPI_SHARED_EXPORT double PPGetLocalTimeZoneOffset(const base::Time& time);

}  

#endif  
