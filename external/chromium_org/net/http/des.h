// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_DES_H_
#define NET_HTTP_DES_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace net {


NET_EXPORT_PRIVATE void DESMakeKey(const uint8* raw, uint8* key);

NET_EXPORT_PRIVATE void DESEncrypt(const uint8* key, const uint8* src,
                                   uint8* hash);

}  

#endif  
