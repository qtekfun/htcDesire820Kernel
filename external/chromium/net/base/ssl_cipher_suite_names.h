// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_SSL_CIPHER_SUITE_NAMES_H_
#define NET_BASE_SSL_CIPHER_SUITE_NAMES_H_
#pragma once

#include "base/basictypes.h"

namespace net {

void SSLCipherSuiteToStrings(const char** key_exchange_str,
                             const char** cipher_str, const char** mac_str,
                             uint16 cipher_suite);

void SSLCompressionToString(const char** name, uint8 compression_method);

void SSLVersionToString(const char** name, int ssl_version);

}  

#endif  
