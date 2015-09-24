// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_DES_H_
#define NET_HTTP_DES_H_
#pragma once

#include "base/basictypes.h"

namespace net {


void DESMakeKey(const uint8* raw, uint8* key);

void DESEncrypt(const uint8* key, const uint8* src, uint8* hash);

}  

#endif  
