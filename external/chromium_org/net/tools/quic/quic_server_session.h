// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_SERVER_SESSION_H_
#define NET_TOOLS_QUIC_QUIC_SERVER_SESSION_H_

#include <set>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "net/quic/quic_crypto_server_stream.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_session.h"

namespace net {

class QuicConfig;
class QuicConnection;
class QuicCryptoServerConfig;
class ReliableQuicStream;

namespace tools {

namespace test {
class QuicServerSessionPeer;
}  

class QuicSessionOwner {
 public:
  virtual ~QuicSessionOwner() {}

  virtual void OnConnectionClosed(QuicGuid guid, QuicErrorCode error) = 0;
};

class QuicServerSession : public QuicSession {
 public:
  QuicServerSession(const QuicConfig& config,
                    QuicConnection *connection,
                    QuicSessionOwner* owner);

  
  virtual void OnConnectionClosed(QuicErrorCode error, bool from_peer) OVERRIDE;

  virtual ~QuicServerSession();

  virtual void InitializeSession(const QuicCryptoServerConfig& crypto_config);

  const QuicCryptoServerStream* crypto_stream() { return crypto_stream_.get(); }

 protected:
  
  virtual QuicDataStream* CreateIncomingDataStream(QuicStreamId id) OVERRIDE;
  virtual QuicDataStream* CreateOutgoingDataStream() OVERRIDE;
  virtual QuicCryptoServerStream* GetCryptoStream() OVERRIDE;

  
  
  
  virtual bool ShouldCreateIncomingDataStream(QuicStreamId id);

  virtual QuicCryptoServerStream* CreateQuicCryptoServerStream(
    const QuicCryptoServerConfig& crypto_config);

 private:
  friend class test::QuicServerSessionPeer;

  scoped_ptr<QuicCryptoServerStream> crypto_stream_;
  QuicSessionOwner* owner_;

  DISALLOW_COPY_AND_ASSIGN(QuicServerSession);
};

}  
}  

#endif  
