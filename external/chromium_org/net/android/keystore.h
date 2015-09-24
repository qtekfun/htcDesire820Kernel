// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_ANDROID_KEYSTORE_H
#define NET_ANDROID_KEYSTORE_H

#include <jni.h>

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/ssl/ssl_client_cert_type.h"

typedef struct evp_pkey_st EVP_PKEY;


namespace net {
namespace android {

enum PrivateKeyType {
#define DEFINE_PRIVATE_KEY_TYPE(name,value)  PRIVATE_KEY_TYPE_ ## name = value,
#include "net/android/private_key_type_list.h"
#undef DEFINE_PRIVATE_KEY_TYPE
};

NET_EXPORT bool GetRSAKeyModulus(jobject private_key,
                                 std::vector<uint8>* modulus);

NET_EXPORT bool GetDSAKeyParamQ(jobject private_key,
                                std::vector<uint8>* q);

bool GetECKeyOrder(jobject private_key,
                   std::vector<uint8>* order);

bool GetPrivateKeyEncodedBytes(jobject private_key,
                               std::vector<uint8>* encoded);

NET_EXPORT bool RawSignDigestWithPrivateKey(
    jobject private_key,
    const base::StringPiece& digest,
    std::vector<uint8>* signature);


NET_EXPORT PrivateKeyType GetPrivateKeyType(jobject private_key);

EVP_PKEY* GetOpenSSLSystemHandleForPrivateKey(jobject private_key);

NET_EXPORT bool RegisterKeyStore(JNIEnv* env);

}  
}  

#endif  
