// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_QUIC_CRYPTO_SERVER_CONFIG_H_
#define NET_QUIC_CRYPTO_QUIC_CRYPTO_SERVER_CONFIG_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "base/synchronization/lock.h"
#include "net/base/ip_endpoint.h"
#include "net/base/net_export.h"
#include "net/quic/crypto/crypto_handshake.h"
#include "net/quic/crypto/crypto_protocol.h"
#include "net/quic/crypto/crypto_secret_boxer.h"
#include "net/quic/quic_time.h"

namespace net {

class EphemeralKeySource;
class KeyExchange;
class ProofSource;
class QuicClock;
class QuicDecrypter;
class QuicEncrypter;
class QuicRandom;
class QuicServerConfigProtobuf;
class StrikeRegister;
class StrikeRegisterClient;

struct ClientHelloInfo;

namespace test {
class QuicCryptoServerConfigPeer;
}  

class NET_EXPORT_PRIVATE ValidateClientHelloResultCallback {
 public:
  
  
  struct Result;

  ValidateClientHelloResultCallback();
  virtual ~ValidateClientHelloResultCallback();
  void Run(const Result* result);

 protected:
  virtual void RunImpl(const CryptoHandshakeMessage& client_hello,
                       const Result& result) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ValidateClientHelloResultCallback);
};

class NET_EXPORT_PRIVATE QuicCryptoServerConfig {
 public:
  
  struct NET_EXPORT_PRIVATE ConfigOptions {
    ConfigOptions();

    
    
    QuicWallTime expiry_time;
    
    
    bool channel_id_enabled;
    
    
    std::string id;
    
    
    std::string orbit;
    
    
    
    
    bool p256;
  };

  
  
  
  
  
  
  QuicCryptoServerConfig(base::StringPiece source_address_token_secret,
                         QuicRandom* server_nonce_entropy);
  ~QuicCryptoServerConfig();

  
  static const char TESTING[];

  
  
  static QuicServerConfigProtobuf* DefaultConfig(
      QuicRandom* rand,
      const QuicClock* clock,
      const ConfigOptions& options);

  
  
  
  
  
  CryptoHandshakeMessage* AddConfig(QuicServerConfigProtobuf* protobuf,
                                    QuicWallTime now);

  
  
  
  CryptoHandshakeMessage* AddDefaultConfig(
      QuicRandom* rand,
      const QuicClock* clock,
      const ConfigOptions& options);

  
  
  
  
  
  
  
  bool SetConfigs(const std::vector<QuicServerConfigProtobuf*>& protobufs,
                  QuicWallTime now);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void ValidateClientHello(
      const CryptoHandshakeMessage& client_hello,
      IPEndPoint client_ip,
      const QuicClock* clock,
      ValidateClientHelloResultCallback* done_cb) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  QuicErrorCode ProcessClientHello(
      const ValidateClientHelloResultCallback::Result& validate_chlo_result,
      QuicGuid guid,
      IPEndPoint client_ip,
      QuicVersion version,
      const QuicVersionVector& supported_versions,
      const QuicClock* clock,
      QuicRandom* rand,
      QuicCryptoNegotiatedParameters* params,
      CryptoHandshakeMessage* out,
      std::string* error_details) const;

  
  
  void SetProofSource(ProofSource* proof_source);

  
  
  
  
  void SetEphemeralKeySource(EphemeralKeySource* ephemeral_key_source);

  
  
  
  void SetStrikeRegisterClient(StrikeRegisterClient* strike_register_client);

  
  
  
  
  
  void set_replay_protection(bool on);

  
  
  void set_strike_register_no_startup_period();

  
  
  
  void set_strike_register_max_entries(uint32 max_entries);

  
  
  
  
  void set_strike_register_window_secs(uint32 window_secs);

  
  
  
  
  void set_source_address_token_future_secs(uint32 future_secs);

  
  
  void set_source_address_token_lifetime_secs(uint32 lifetime_secs);

  
  
  
  
  
  
  void set_server_nonce_strike_register_max_entries(uint32 max_entries);

  
  
  
  
  
  void set_server_nonce_strike_register_window_secs(uint32 window_secs);

 private:
  friend class test::QuicCryptoServerConfigPeer;

  
  
  class NET_EXPORT_PRIVATE Config : public QuicCryptoConfig,
                                    public base::RefCounted<Config> {
   public:
    Config();

    
    
    
    
    std::string serialized;
    
    std::string id;
    
    
    unsigned char orbit[kOrbitSize];

    
    
    
    std::vector<KeyExchange*> key_exchanges;

    
    QuicTagValueMap tag_value_map;

    
    
    bool channel_id_enabled;

    
    
    bool is_primary;

    
    
    
    QuicWallTime primary_time;

   private:
    friend class base::RefCounted<Config>;
    virtual ~Config();

    DISALLOW_COPY_AND_ASSIGN(Config);
  };

  typedef std::map<ServerConfigID, scoped_refptr<Config> > ConfigMap;

  
  
  static bool ConfigPrimaryTimeLessThan(const scoped_refptr<Config>& a,
                                        const scoped_refptr<Config>& b);

  
  
  void SelectNewPrimaryConfig(QuicWallTime now) const;

  
  
  // written to |info|.
  void EvaluateClientHello(
      const uint8* primary_orbit,
      ValidateClientHelloResultCallback::Result* client_hello_state,
      ValidateClientHelloResultCallback* done_cb) const;

  
  void BuildRejection(
      const scoped_refptr<Config>& config,
      const CryptoHandshakeMessage& client_hello,
      const ClientHelloInfo& info,
      QuicRandom* rand,
      CryptoHandshakeMessage* out) const;

  
  
  
  scoped_refptr<Config> ParseConfigProtobuf(QuicServerConfigProtobuf* protobuf);

  
  
  std::string NewSourceAddressToken(const IPEndPoint& ip,
                                    QuicRandom* rand,
                                    QuicWallTime now) const;

  
  
  
  bool ValidateSourceAddressToken(base::StringPiece token,
                                  const IPEndPoint& ip,
                                  QuicWallTime now) const;

  
  std::string NewServerNonce(QuicRandom* rand, QuicWallTime now) const;

  
  
  
  
  
  bool ValidateServerNonce(base::StringPiece echoed_server_nonce,
                           QuicWallTime now) const;

  
  
  bool replay_protection_;

  
  
  
  
  mutable base::Lock configs_lock_;
  
  
  ConfigMap configs_;
  
  
  mutable scoped_refptr<Config> primary_config_;
  
  
  mutable QuicWallTime next_config_promotion_time_;

  mutable base::Lock strike_register_client_lock_;
  
  
  mutable scoped_ptr<StrikeRegisterClient> strike_register_client_;

  
  
  CryptoSecretBoxer source_address_token_boxer_;

  
  
  CryptoSecretBoxer server_nonce_boxer_;

  
  
  
  uint8 server_nonce_orbit_[8];

  mutable base::Lock server_nonce_strike_register_lock_;
  
  
  
  mutable scoped_ptr<StrikeRegister> server_nonce_strike_register_;

  
  
  scoped_ptr<ProofSource> proof_source_;

  
  
  scoped_ptr<EphemeralKeySource> ephemeral_key_source_;

  
  
  bool strike_register_no_startup_period_;
  uint32 strike_register_max_entries_;
  uint32 strike_register_window_secs_;
  uint32 source_address_token_future_secs_;
  uint32 source_address_token_lifetime_secs_;
  uint32 server_nonce_strike_register_max_entries_;
  uint32 server_nonce_strike_register_window_secs_;
};

}  

#endif  
