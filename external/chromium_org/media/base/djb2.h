// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_DJB2_H_
#define MEDIA_BASE_DJB2_H_

#include "base/basictypes.h"
#include "media/base/media_export.h"


static const uint32 kDJB2HashSeed = 5381u;


MEDIA_EXPORT uint32 DJB2Hash(const void* buf, size_t len, uint32 seed);

#endif  

