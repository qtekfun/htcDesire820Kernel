// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SSL_SSL_CIPHER_SUITE_NAMES_H_
#define NET_SSL_SSL_CIPHER_SUITE_NAMES_H_

#include <string>

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace net {

NET_EXPORT void SSLCipherSuiteToStrings(const char** key_exchange_str,
                                        const char** cipher_str,
                                        const char** mac_str,
                                        bool* is_aead,
                                        uint16 cipher_suite);

NET_EXPORT void SSLVersionToString(const char** name, int ssl_version);

NET_EXPORT bool ParseSSLCipherString(const std::string& cipher_string,
                                     uint16* cipher_suite);

}  

#endif  
