// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_CLIENT_SESSION_H_
#define NET_QUIC_QUIC_CLIENT_SESSION_H_

#include <string>

#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/completion_callback.h"
#include "net/quic/quic_connection_logger.h"
#include "net/quic/quic_crypto_client_stream.h"
#include "net/quic/quic_reliable_client_stream.h"
#include "net/quic/quic_session.h"

namespace net {

class DatagramClientSocket;
class QuicConnectionHelper;
class QuicCryptoClientStreamFactory;
class QuicDefaultPacketWriter;
class QuicStreamFactory;
class SSLInfo;

namespace test {
class QuicClientSessionPeer;
}  

class NET_EXPORT_PRIVATE QuicClientSession : public QuicSession {
 public:
  
  class NET_EXPORT_PRIVATE Observer {
   public:
    virtual ~Observer() {}
    virtual void OnCryptoHandshakeConfirmed() = 0;
    virtual void OnSessionClosed(int error) = 0;
  };

  
  class NET_EXPORT_PRIVATE StreamRequest {
   public:
    StreamRequest();
    ~StreamRequest();

    
    
    
    
    int StartRequest(const base::WeakPtr<QuicClientSession>& session,
                     QuicReliableClientStream** stream,
                     const CompletionCallback& callback);

    
    
    void CancelRequest();

   private:
    friend class QuicClientSession;

    
    
    void OnRequestCompleteSuccess(QuicReliableClientStream* stream);

    
    
    
    void OnRequestCompleteFailure(int rv);

    base::WeakPtr<QuicClientSession> session_;
    CompletionCallback callback_;
    QuicReliableClientStream** stream_;

    DISALLOW_COPY_AND_ASSIGN(StreamRequest);
  };

  
  
  
  QuicClientSession(QuicConnection* connection,
                    scoped_ptr<DatagramClientSocket> socket,
                    scoped_ptr<QuicDefaultPacketWriter> writer,
                    QuicStreamFactory* stream_factory,
                    QuicCryptoClientStreamFactory* crypto_client_stream_factory,
                    const std::string& server_hostname,
                    const QuicConfig& config,
                    QuicCryptoClientConfig* crypto_config,
                    NetLog* net_log);

  virtual ~QuicClientSession();

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  
  
  
  
  
  
  int TryCreateStream(StreamRequest* request,
                      QuicReliableClientStream** stream);

  
  void CancelRequest(StreamRequest* request);

  
  virtual bool OnStreamFrames(
      const std::vector<QuicStreamFrame>& frames) OVERRIDE;
  virtual QuicReliableClientStream* CreateOutgoingDataStream() OVERRIDE;
  virtual QuicCryptoClientStream* GetCryptoStream() OVERRIDE;
  virtual void CloseStream(QuicStreamId stream_id) OVERRIDE;
  virtual void SendRstStream(QuicStreamId id,
                             QuicRstStreamErrorCode error) OVERRIDE;
  virtual void OnCryptoHandshakeEvent(CryptoHandshakeEvent event) OVERRIDE;
  virtual void OnCryptoHandshakeMessageSent(
      const CryptoHandshakeMessage& message) OVERRIDE;
  virtual void OnCryptoHandshakeMessageReceived(
      const CryptoHandshakeMessage& message) OVERRIDE;
  virtual bool GetSSLInfo(SSLInfo* ssl_info) OVERRIDE;

  
  virtual void OnConnectionClosed(QuicErrorCode error, bool from_peer) OVERRIDE;
  virtual void OnSuccessfulVersionNegotiation(
      const QuicVersion& version) OVERRIDE;

  
  int CryptoConnect(bool require_confirmation,
                    const CompletionCallback& callback);

  
  
  void StartReading();

  
  
  void CloseSessionOnError(int error);

  base::Value* GetInfoAsValue(const HostPortPair& pair) const;

  const BoundNetLog& net_log() const { return net_log_; }

  base::WeakPtr<QuicClientSession> GetWeakPtr();

  
  
  
  int GetNumSentClientHellos() const;

 protected:
  
  virtual QuicDataStream* CreateIncomingDataStream(QuicStreamId id) OVERRIDE;

 private:
  friend class test::QuicClientSessionPeer;

  typedef std::set<Observer*> ObserverSet;
  typedef std::list<StreamRequest*> StreamRequestQueue;

  QuicReliableClientStream* CreateOutgoingReliableStreamImpl();
  
  void OnReadComplete(int result);

  void OnClosedStream();

  
  
  
  
  
  
  
  void CloseSessionOnErrorInner(int net_error, QuicErrorCode quic_error);

  void CloseAllStreams(int net_error);
  void CloseAllObservers(int net_error);

  
  
  
  void NotifyFactoryOfSessionGoingAway();

  
  void NotifyFactoryOfSessionClosedLater();

  
  
  void NotifyFactoryOfSessionClosed();

  bool require_confirmation_;
  scoped_ptr<QuicCryptoClientStream> crypto_stream_;
  QuicStreamFactory* stream_factory_;
  scoped_ptr<DatagramClientSocket> socket_;
  scoped_ptr<QuicDefaultPacketWriter> writer_;
  scoped_refptr<IOBufferWithSize> read_buffer_;
  ObserverSet observers_;
  StreamRequestQueue stream_requests_;
  bool read_pending_;
  CompletionCallback callback_;
  size_t num_total_streams_;
  BoundNetLog net_log_;
  QuicConnectionLogger logger_;
  
  size_t num_packets_read_;
  base::WeakPtrFactory<QuicClientSession> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(QuicClientSession);
};

}  

#endif  
