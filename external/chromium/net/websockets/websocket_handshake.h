// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_H_
#define NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "googleurl/src/gurl.h"

namespace net {

class HttpResponseHeaders;

class WebSocketHandshake {
 public:
  static const int kWebSocketPort;
  static const int kSecureWebSocketPort;

  enum Mode {
    MODE_INCOMPLETE, MODE_NORMAL, MODE_FAILED, MODE_CONNECTED
  };
  WebSocketHandshake(const GURL& url,
                     const std::string& origin,
                     const std::string& location,
                     const std::string& protocol);
  virtual ~WebSocketHandshake();

  bool is_secure() const;
  
  virtual std::string CreateClientHandshakeMessage();

  
  
  
  
  
  virtual int ReadServerHandshake(const char* data, size_t len);
  Mode mode() const { return mode_; }

 protected:
  std::string GetResourceName() const;
  std::string GetHostFieldValue() const;
  std::string GetOriginFieldValue() const;

  
  
  
  
  
  static bool GetSingleHeader(const HttpResponseHeaders& headers,
                              const std::string& name,
                              std::string* value);

  GURL url_;
  
  std::string origin_;
  std::string location_;
  std::string protocol_;

  Mode mode_;

  
  std::string ws_origin_;
  std::string ws_location_;
  std::string ws_protocol_;

 private:
  friend class WebSocketHandshakeTest;

  class Parameter {
   public:
    static const int kKey3Size = 8;
    static const int kExpectedResponseSize = 16;
    Parameter();
    ~Parameter();

    void GenerateKeys();
    const std::string& GetSecWebSocketKey1() const { return key_1_; }
    const std::string& GetSecWebSocketKey2() const { return key_2_; }
    const std::string& GetKey3() const { return key_3_; }

    void GetExpectedResponse(uint8* expected) const;

   private:
    friend class WebSocketHandshakeTest;

    
    
    static void SetRandomNumberGenerator(
        uint32 (*rand)(uint32 min, uint32 max));
    void GenerateSecWebSocketKey(uint32* number, std::string* key);
    void GenerateKey3();

    uint32 number_1_;
    uint32 number_2_;
    std::string key_1_;
    std::string key_2_;
    std::string key_3_;

    static uint32 (*rand_)(uint32 min, uint32 max);
  };

  virtual bool ProcessHeaders(const HttpResponseHeaders& headers);
  virtual bool CheckResponseHeaders() const;

  scoped_ptr<Parameter> parameter_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketHandshake);
};

}  

#endif  
