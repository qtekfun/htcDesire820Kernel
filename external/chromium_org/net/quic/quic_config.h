// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_CONFIG_H_
#define NET_QUIC_QUIC_CONFIG_H_

#include <string>

#include "base/basictypes.h"
#include "net/quic/crypto/crypto_handshake.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_time.h"
#include "net/quic/quic_utils.h"

namespace net {

class NET_EXPORT_PRIVATE QuicNegotiableValue {
 public:
  enum Presence {
    
    
    PRESENCE_OPTIONAL,
    
    
    PRESENCE_REQUIRED,
  };

  QuicNegotiableValue(QuicTag tag, Presence presence);

  bool negotiated() const {
    return negotiated_;
  }

 protected:
  const QuicTag tag_;
  const Presence presence_;
  bool negotiated_;
};

class NET_EXPORT_PRIVATE QuicNegotiableUint32 : public QuicNegotiableValue {
 public:
  
  QuicNegotiableUint32(QuicTag name, Presence presence);

  
  
  
  
  void set(uint32 max, uint32 default_value);

  
  
  uint32 GetUint32() const;

  
  
  void ToHandshakeMessage(CryptoHandshakeMessage* out) const;

  
  
  
  
  QuicErrorCode ProcessClientHello(const CryptoHandshakeMessage& client_hello,
                                   std::string* error_details);

  
  
  
  
  QuicErrorCode ProcessServerHello(const CryptoHandshakeMessage& server_hello,
                                   std::string* error_details);

 private:
  
  
  
  QuicErrorCode ReadUint32(const CryptoHandshakeMessage& msg,
                           uint32* out,
                           std::string* error_details) const;

  uint32 max_value_;
  uint32 default_value_;
  uint32 negotiated_value_;
};

class NET_EXPORT_PRIVATE QuicNegotiableTag : public QuicNegotiableValue {
 public:
  QuicNegotiableTag(QuicTag name, Presence presence);
  ~QuicNegotiableTag();

  
  
  
  void set(const QuicTagVector& possible_values, QuicTag default_value);

  
  
  QuicTag GetTag() const;

  
  
  
  void ToHandshakeMessage(CryptoHandshakeMessage* out) const;

  
  
  
  QuicErrorCode ProcessClientHello(const CryptoHandshakeMessage& client_hello,
                                   std::string* error_details);

  
  
  
  QuicErrorCode ProcessServerHello(const CryptoHandshakeMessage& server_hello,
                                   std::string* error_details);

 private:
  
  
  
  QuicErrorCode ReadVector(const CryptoHandshakeMessage& msg,
                           const QuicTag** out,
                           size_t* out_length,
                           std::string* error_details) const;

  QuicTag negotiated_tag_;
  QuicTagVector possible_values_;
  QuicTag default_value_;
};

class NET_EXPORT_PRIVATE QuicConfig {
 public:
  QuicConfig();
  ~QuicConfig();

  void set_congestion_control(const QuicTagVector& congestion_control,
                              QuicTag default_congestion_control);

  QuicTag congestion_control() const;

  void set_idle_connection_state_lifetime(
      QuicTime::Delta max_idle_connection_state_lifetime,
      QuicTime::Delta default_idle_conection_state_lifetime);

  QuicTime::Delta idle_connection_state_lifetime() const;

  QuicTime::Delta keepalive_timeout() const;

  void set_max_streams_per_connection(size_t max_streams,
                                      size_t default_streams);

  uint32 max_streams_per_connection() const;

  void set_max_time_before_crypto_handshake(
      QuicTime::Delta max_time_before_crypto_handshake);

  QuicTime::Delta max_time_before_crypto_handshake() const;

  
  
  void set_server_initial_congestion_window(size_t max_initial_window,
                                            size_t default_initial_window);

  uint32 server_initial_congestion_window() const;

  
  void set_initial_round_trip_time_us(size_t max_rtt, size_t default_rtt);

  uint32 initial_round_trip_time_us() const;

  bool negotiated();

  
  void SetDefaults();

  
  
  void ToHandshakeMessage(CryptoHandshakeMessage* out) const;

  
  
  QuicErrorCode ProcessClientHello(const CryptoHandshakeMessage& client_hello,
                                   std::string* error_details);

  
  
  QuicErrorCode ProcessServerHello(const CryptoHandshakeMessage& server_hello,
                                   std::string* error_details);

 private:
  
  QuicNegotiableTag congestion_control_;
  
  QuicNegotiableUint32 idle_connection_state_lifetime_seconds_;
  
  QuicNegotiableUint32 keepalive_timeout_seconds_;
  
  QuicNegotiableUint32 max_streams_per_connection_;
  
  
  QuicTime::Delta max_time_before_crypto_handshake_;
  
  QuicNegotiableUint32 server_initial_congestion_window_;
  
  QuicNegotiableUint32 initial_round_trip_time_us_;
};

}  

#endif  
