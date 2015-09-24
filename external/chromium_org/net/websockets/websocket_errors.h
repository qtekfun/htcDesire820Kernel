// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_ERRORS_H_
#define NET_WEBSOCKETS_WEBSOCKET_ERRORS_H_

#include "net/base/net_errors.h"

namespace net {

enum WebSocketError {
  

  
  kWebSocketNormalClosure = 1000,
  kWebSocketErrorGoingAway = 1001,
  kWebSocketErrorProtocolError = 1002,
  kWebSocketErrorUnsupportedData = 1003,
  kWebSocketErrorNoStatusReceived = 1005,
  kWebSocketErrorAbnormalClosure = 1006,
  kWebSocketErrorInvalidFramePayloadData = 1007,
  kWebSocketErrorPolicyViolation = 1008,
  kWebSocketErrorMessageTooBig = 1009,
  kWebSocketErrorMandatoryExtension = 1010,
  kWebSocketErrorInternalServerError = 1011,
  kWebSocketErrorTlsHandshake = 1015,

  
  
  kWebSocketErrorProtocolReservedMax = 2999,

  
  
  kWebSocketErrorRegisteredReservedMin = 3000,
  kWebSocketErrorRegisteredReservedMax = 3999,

  
  
  kWebSocketErrorPrivateReservedMin = 4000,
  kWebSocketErrorPrivateReservedMax = 4999,
};

NET_EXPORT_PRIVATE Error WebSocketErrorToNetError(WebSocketError error);

}  

#endif  
