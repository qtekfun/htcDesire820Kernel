// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_NSS_UTIL_INTERNAL_H_
#define CRYPTO_NSS_UTIL_INTERNAL_H_
#pragma once

#include <secmodt.h>


namespace crypto {

PK11SlotInfo* GetPublicNSSKeySlot();

PK11SlotInfo* GetPrivateNSSKeySlot();

}  

#endif  
