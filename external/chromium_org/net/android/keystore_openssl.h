// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_ANDROID_KEYSTORE_OPENSSL_H
#define NET_ANDROID_KEYSTORE_OPENSSL_H

#include <jni.h>
#include <openssl/evp.h>

#include "net/base/net_export.h"


namespace net {
namespace android {

NET_EXPORT EVP_PKEY* GetOpenSSLPrivateKeyWrapper(jobject private_key);

}  
}  

#endif  
