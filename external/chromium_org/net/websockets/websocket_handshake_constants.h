// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_CONSTANTS_H_
#define NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_CONSTANTS_H_

#include "base/basictypes.h"

namespace net {
namespace websockets {

extern const char* const kHttpProtocolVersion;

extern const size_t kRawChallengeLength;

extern const char* const kSecWebSocketProtocol;

extern const char* const kSecWebSocketExtensions;

extern const char* const kSecWebSocketKey;

extern const char* const kSecWebSocketAccept;

extern const char* const kSecWebSocketVersion;

extern const char* const kSupportedVersion;

extern const char* const kUpgrade;

extern const char* const kWebSocketGuid;


extern const char* const kSecWebSocketProtocolSpdy3;

extern const char* const kSecWebSocketExtensionsSpdy3;

extern const char* const kSecWebSocketProtocolLowercase;

extern const char* const kSecWebSocketExtensionsLowercase;

extern const char* const kSecWebSocketKeyLowercase;

extern const char* const kSecWebSocketVersionLowercase;

extern const char* const kUpgradeLowercase;

extern const char* const kWebSocketLowercase;

}  
}  

#endif  
