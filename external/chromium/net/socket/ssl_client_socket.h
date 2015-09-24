// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_CLIENT_SOCKET_H_
#define NET_SOCKET_SSL_CLIENT_SOCKET_H_
#pragma once

#include <string>

#include "net/base/completion_callback.h"
#include "net/base/load_flags.h"
#include "net/base/net_errors.h"
#include "net/socket/client_socket.h"

namespace net {

class SSLCertRequestInfo;
class SSLHostInfo;
class SSLInfo;
struct RRResponse;

class DNSSECProvider {
 public:
  
  
  
  
  
  
  virtual int GetDNSSECRecords(RRResponse** out,
                               CompletionCallback* callback) = 0;

 private:
  ~DNSSECProvider() {}
};

class SSLClientSocket : public ClientSocket {
 public:
  SSLClientSocket();

  
  
  
  enum NextProtoStatus {
    

    kNextProtoUnsupported = 0,  
    kNextProtoNegotiated = 1,   
    kNextProtoNoOverlap = 2,    
                                
  };

  
  
  
  
  enum NextProto {
    kProtoUnknown = 0,
    kProtoHTTP11 = 1,
    kProtoSPDY1 = 2,
    kProtoSPDY2 = 3,
  };

  
  virtual void GetSSLInfo(SSLInfo* ssl_info) = 0;

  
  
  virtual void GetSSLCertRequestInfo(
      SSLCertRequestInfo* cert_request_info) = 0;

  
  
  
  
  
  
  
  virtual NextProtoStatus GetNextProto(std::string* proto) = 0;

  static NextProto NextProtoFromString(const std::string& proto_string);

  static bool IgnoreCertError(int error, int load_flags);

  virtual bool was_npn_negotiated() const;

  virtual bool set_was_npn_negotiated(bool negotiated);

  virtual void UseDNSSEC(DNSSECProvider*) { }

  virtual bool was_spdy_negotiated() const;

  virtual bool set_was_spdy_negotiated(bool negotiated);

 private:
  
  bool was_npn_negotiated_;
  
  bool was_spdy_negotiated_;
};

}  

#endif  
