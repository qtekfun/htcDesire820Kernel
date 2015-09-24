// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_NEXT_PROTO_H_
#define NET_SOCKET_NEXT_PROTO_H_

namespace net {

enum NextProto {
  kProtoUnknown = 0,
  kProtoHTTP11,
  kProtoMinimumVersion = kProtoHTTP11,

  kProtoDeprecatedSPDY2,
  kProtoSPDYMinimumVersion = kProtoDeprecatedSPDY2,
  kProtoSPDY3,
  kProtoSPDY31,
  kProtoSPDY4a2,
  
  kProtoHTTP2Draft04,
  kProtoSPDYMaximumVersion = kProtoHTTP2Draft04,

  kProtoQUIC1SPDY3,

  kProtoMaximumVersion = kProtoQUIC1SPDY3,
};

}  

#endif  
