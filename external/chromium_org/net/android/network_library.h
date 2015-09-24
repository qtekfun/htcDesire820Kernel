// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_ANDROID_NETWORK_LIBRARY_H_
#define NET_ANDROID_NETWORK_LIBRARY_H_

#include <jni.h>

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "net/android/cert_verify_result_android.h"
#include "net/base/mime_util.h"
#include "net/base/net_export.h"

namespace net {
namespace android {

CertVerifyResultAndroid VerifyX509CertChain(
    const std::vector<std::string>& cert_chain,
    const std::string& auth_type);

void AddTestRootCertificate(const uint8* cert, size_t len);

void ClearTestRootCertificates();

bool StoreKeyPair(const uint8* public_key,
                  size_t public_len,
                  const uint8* private_key,
                  size_t private_len);

NET_EXPORT void StoreCertificate(net::CertificateMimeType cert_type,
                                 const void* data,
                                 size_t data_len);

bool HaveOnlyLoopbackAddresses();

std::string GetNetworkList();

bool GetMimeTypeFromExtension(const std::string& extension,
                              std::string* result);

NET_EXPORT bool RegisterNetworkLibrary(JNIEnv* env);

}  
}  

#endif  
