// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_CAST_CHANNEL_CAST_SOCKET_H_
#define CHROME_BROWSER_EXTENSIONS_API_CAST_CHANNEL_CAST_SOCKET_H_

#include <queue>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "chrome/browser/extensions/api/api_resource.h"
#include "chrome/browser/extensions/api/api_resource_manager.h"
#include "chrome/common/extensions/api/cast_channel.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/base/ip_endpoint.h"
#include "net/base/net_log.h"
#include "url/gurl.h"

namespace net {
class AddressList;
class CertVerifier;
class SSLClientSocket;
class TCPClientSocket;
class TransportSecurityState;
}

namespace extensions {
namespace api {
namespace cast_channel {

class CastMessage;

extern const uint32 kMaxMessageSize;

class CastSocket : public ApiResource,
                   public base::SupportsWeakPtr<CastSocket> {
 public:
  
  class Delegate {
   public:
    
    virtual void OnError(const CastSocket* socket,
                         ChannelError error) = 0;
    
    virtual void OnMessage(const CastSocket* socket,
                           const MessageInfo& message) = 0;
   protected:
    virtual ~Delegate() {}
  };

  
  
  CastSocket(const std::string& owner_extension_id,
             const GURL& url,
             CastSocket::Delegate* delegate,
             net::NetLog* net_log);
  virtual ~CastSocket();

  
  const GURL& url() const;

  
  bool auth_required() const { return auth_required_; }

  
  int id() const { return channel_id_; }

  
  void set_id(int channel_id) { channel_id_ = channel_id; }

  
  ReadyState ready_state() const { return ready_state_; }

  
  
  ChannelError error_state() const { return error_state_; }

  
  
  virtual void Connect(const net::CompletionCallback& callback);

  
  
  virtual void SendMessage(const MessageInfo& message,
                           const net::CompletionCallback& callback);

  
  
  virtual void Close(const net::CompletionCallback& callback);

  
  virtual void FillChannelInfo(ChannelInfo* channel_info) const;

 protected:
  
  virtual scoped_ptr<net::TCPClientSocket> CreateTcpSocket();
  
  virtual scoped_ptr<net::SSLClientSocket> CreateSslSocket();
  
  
  
  virtual bool ExtractPeerCert(std::string* cert);
  
  virtual int SendAuthChallenge();
  
  virtual int ReadAuthChallengeReply();
  
  
  
  virtual bool VerifyChallengeReply();

  
  virtual bool CalledOnValidThread() const;

 private:
  friend class ApiResourceManager<CastSocket>;
  friend class CastSocketTest;

  static const char* service_name() {
    return "CastSocketManager";
  }

  
  enum ConnectionState {
    CONN_STATE_NONE,
    CONN_STATE_TCP_CONNECT,
    CONN_STATE_TCP_CONNECT_COMPLETE,
    CONN_STATE_SSL_CONNECT,
    CONN_STATE_SSL_CONNECT_COMPLETE,
    CONN_STATE_AUTH_CHALLENGE_SEND,
    CONN_STATE_AUTH_CHALLENGE_SEND_COMPLETE,
    CONN_STATE_AUTH_CHALLENGE_REPLY_COMPLETE,
  };

  
  
  
  
  
  
  
  
  
  

  
  int DoConnectLoop(int result);
  
  
  
  int DoTcpConnect();
  int DoTcpConnectComplete(int result);
  int DoSslConnect();
  int DoSslConnectComplete(int result);
  int DoAuthChallengeSend();
  int DoAuthChallengeSendComplete(int result);
  int DoAuthChallengeReplyComplete(int result);
  

  
  void OnConnectComplete(int result);

  
  void OnChallengeEvent(int result);

  
  void DoConnectCallback(int result);

  
  
  bool ParseChannelUrl(const GURL& url);

  
  int SendMessageInternal(const CastMessage& message,
                          const net::CompletionCallback& callback);

  
  
  int WriteData();
  void OnWriteData(int result);

  
  
  int ReadData();
  void OnReadData(int result);

  
  bool ProcessHeader();
  
  bool ProcessBody();
  
  
  bool ParseMessageFromBody();

  
  static bool Serialize(const CastMessage& message_proto,
                        std::string* message_data);

  
  
  void CloseWithError(ChannelError error);

  base::ThreadChecker thread_checker_;

  
  int channel_id_;

  
  GURL url_;
  
  Delegate* delegate_;
  
  bool auth_required_;
  
  net::IPEndPoint ip_endpoint_;
  
  ChannelError error_state_;
  
  ReadyState ready_state_;

  
  bool write_callback_pending_;
  
  bool read_callback_pending_;

  
  scoped_refptr<net::GrowableIOBuffer> header_read_buffer_;
  
  scoped_refptr<net::GrowableIOBuffer> body_read_buffer_;
  
  scoped_refptr<net::GrowableIOBuffer> current_read_buffer_;
  
  uint32 current_message_size_;

  
  net::NetLog* net_log_;
  
  net::NetLog::Source net_log_source_;

  
  ConnectionState next_state_;
  
  scoped_ptr<net::TCPClientSocket> tcp_socket_;
  
  scoped_ptr<net::SSLClientSocket> socket_;
  
  
  std::string peer_cert_;
  scoped_ptr<net::CertVerifier> cert_verifier_;
  scoped_ptr<net::TransportSecurityState> transport_security_state_;
  
  scoped_ptr<CastMessage> challenge_reply_;

  
  net::CompletionCallback connect_callback_;

  
  
  struct MessageHeader {
    MessageHeader();
    
    void SetMessageSize(size_t message_size);
    
    void PrependToString(std::string* str);
    
    static void ReadFromIOBuffer(net::GrowableIOBuffer* buffer,
                                 MessageHeader* header);
    std::string ToString();
    
    uint32 message_size;
  };

  // Holds a message to be written to the socket. |callback| is invoked when the
  // message is fully written or an error occurrs.
  struct WriteRequest {
    explicit WriteRequest(const net::CompletionCallback& callback);
    ~WriteRequest();
    
    
    bool SetContent(const CastMessage& message_proto);

    net::CompletionCallback callback;
    scoped_refptr<net::DrainableIOBuffer> io_buffer;
  };
  
  // being written.
  std::queue<WriteRequest> write_queue_;

  
  bool in_connect_loop_;

  FRIEND_TEST_ALL_PREFIXES(CastSocketTest, TestCastURLs);
  FRIEND_TEST_ALL_PREFIXES(CastSocketTest, TestRead);
  FRIEND_TEST_ALL_PREFIXES(CastSocketTest, TestReadMany);
  DISALLOW_COPY_AND_ASSIGN(CastSocket);
};

}  
}  
}  

#endif  
