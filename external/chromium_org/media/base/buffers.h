// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_BASE_BUFFERS_H_
#define MEDIA_BASE_BUFFERS_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "media/base/media_export.h"

namespace media {


MEDIA_EXPORT extern inline base::TimeDelta kNoTimestamp() {
  return base::TimeDelta::FromMicroseconds(kint64min);
}

MEDIA_EXPORT extern inline base::TimeDelta kInfiniteDuration() {
  return base::TimeDelta::FromMicroseconds(kint64max);
}

}  

#endif  
