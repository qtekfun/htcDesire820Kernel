// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_AUTH_UTIL_H_
#define REMOTING_PROTOCOL_AUTH_UTIL_H_

#include <string>

#include "base/strings/string_piece.h"

namespace net {
class SSLSocket;
}  

namespace remoting {
namespace protocol {

extern const char kClientAuthSslExporterLabel[];
extern const char kHostAuthSslExporterLabel[];

extern const char kSslFakeHostName[];

const size_t kSharedSecretHashLength = 32;

const size_t kAuthDigestLength = 32;


std::string GenerateSupportAuthToken(const std::string& jid,
                                     const std::string& access_code);

bool VerifySupportAuthToken(const std::string& jid,
                            const std::string& access_code,
                            const std::string& auth_token);

std::string GetAuthBytes(net::SSLSocket* socket,
                         const base::StringPiece& label,
                         const base::StringPiece& shared_secret);

}  
}  

#endif  
