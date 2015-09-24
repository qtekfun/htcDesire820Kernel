// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SOCKET_TEST_UTIL_H_
#define NET_SOCKET_SOCKET_TEST_UTIL_H_
#pragma once

#include <cstring>
#include <deque>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/string16.h"
#include "net/base/address_list.h"
#include "net/base/io_buffer.h"
#include "net/base/net_errors.h"
#include "net/base/net_log.h"
#include "net/base/ssl_config_service.h"
#include "net/base/test_completion_callback.h"
#include "net/http/http_auth_controller.h"
#include "net/http/http_proxy_client_socket_pool.h"
#include "net/socket/client_socket_factory.h"
#include "net/socket/client_socket_handle.h"
#include "net/socket/socks_client_socket_pool.h"
#include "net/socket/ssl_client_socket.h"
#include "net/socket/ssl_client_socket_pool.h"
#include "net/socket/transport_client_socket_pool.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace net {

enum {
  
  
  
  
  
  ERR_TEST_PEER_CLOSE_AFTER_NEXT_MOCK_READ = -10000,
};

class ClientSocket;
class MockClientSocket;
class SSLClientSocket;
class SSLHostInfo;

struct MockConnect {
  
  MockConnect() : async(true), result(OK) { }
  MockConnect(bool a, int r) : async(a), result(r) { }

  bool async;
  int result;
};

struct MockRead {
  
  enum {
    STOPLOOP = 1 << 31
  };

  
  MockRead() : async(false), result(0), data(NULL), data_len(0),
      sequence_number(0), time_stamp(base::Time::Now()) {}

  
  MockRead(bool async, int result) : async(async) , result(result), data(NULL),
      data_len(0), sequence_number(0), time_stamp(base::Time::Now()) { }

  
  MockRead(bool async, int result, int seq) : async(async) , result(result),
      data(NULL), data_len(0), sequence_number(seq),
      time_stamp(base::Time::Now()) { }

  
  explicit MockRead(const char* data) : async(true),  result(0), data(data),
      data_len(strlen(data)), sequence_number(0),
      time_stamp(base::Time::Now()) { }

  
  MockRead(bool async, const char* data) : async(async), result(0), data(data),
      data_len(strlen(data)), sequence_number(0),
      time_stamp(base::Time::Now()) { }

  
  MockRead(bool async, const char* data, int data_len) : async(async),
      result(0), data(data), data_len(data_len), sequence_number(0),
      time_stamp(base::Time::Now()) { }

  
  MockRead(bool async, int seq, const char* data) : async(async),
      result(0), data(data), data_len(strlen(data)), sequence_number(seq),
      time_stamp(base::Time::Now()) { }

  
  MockRead(bool async, const char* data, int data_len, int seq) : async(async),
      result(0), data(data), data_len(data_len), sequence_number(seq),
      time_stamp(base::Time::Now()) { }

  bool async;
  int result;
  const char* data;
  int data_len;

  
  
  
  int sequence_number;      
                            
  base::Time time_stamp;    
};

typedef MockRead MockWrite;

struct MockWriteResult {
  MockWriteResult(bool async, int result) : async(async), result(result) {}

  bool async;
  int result;
};

class SocketDataProvider {
 public:
  SocketDataProvider() : socket_(NULL) {}

  virtual ~SocketDataProvider() {}

  
  
  
  
  virtual MockRead GetNextRead() = 0;
  virtual MockWriteResult OnWrite(const std::string& data) = 0;
  virtual void Reset() = 0;

  
  MockClientSocket* socket() { return socket_; }
  void set_socket(MockClientSocket* socket) { socket_ = socket; }

  MockConnect connect_data() const { return connect_; }
  void set_connect_data(const MockConnect& connect) { connect_ = connect; }

 private:
  MockConnect connect_;
  MockClientSocket* socket_;

  DISALLOW_COPY_AND_ASSIGN(SocketDataProvider);
};

class StaticSocketDataProvider : public SocketDataProvider {
 public:
  StaticSocketDataProvider();
  StaticSocketDataProvider(MockRead* reads, size_t reads_count,
                           MockWrite* writes, size_t writes_count);
  virtual ~StaticSocketDataProvider();

  
  const MockRead& PeekRead() const;
  const MockWrite& PeekWrite() const;
  
  const MockRead& PeekRead(size_t index) const;
  const MockWrite& PeekWrite(size_t index) const;
  size_t read_index() const { return read_index_; }
  size_t write_index() const { return write_index_; }
  size_t read_count() const { return read_count_; }
  size_t write_count() const { return write_count_; }

  bool at_read_eof() const { return read_index_ >= read_count_; }
  bool at_write_eof() const { return write_index_ >= write_count_; }

  virtual void CompleteRead() {}

  
  virtual MockRead GetNextRead();
  virtual MockWriteResult OnWrite(const std::string& data);
  virtual void Reset();

 private:
  MockRead* reads_;
  size_t read_index_;
  size_t read_count_;
  MockWrite* writes_;
  size_t write_index_;
  size_t write_count_;

  DISALLOW_COPY_AND_ASSIGN(StaticSocketDataProvider);
};

class DynamicSocketDataProvider : public SocketDataProvider {
 public:
  DynamicSocketDataProvider();
  virtual ~DynamicSocketDataProvider();

  int short_read_limit() const { return short_read_limit_; }
  void set_short_read_limit(int limit) { short_read_limit_ = limit; }

  void allow_unconsumed_reads(bool allow) { allow_unconsumed_reads_ = allow; }

  
  virtual MockRead GetNextRead();
  virtual MockWriteResult OnWrite(const std::string& data) = 0;
  virtual void Reset();

 protected:
  
  
  void SimulateRead(const char* data, size_t length);
  void SimulateRead(const char* data) {
    SimulateRead(data, std::strlen(data));
  }

 private:
  std::deque<MockRead> reads_;

  
  int short_read_limit_;

  
  
  bool allow_unconsumed_reads_;

  DISALLOW_COPY_AND_ASSIGN(DynamicSocketDataProvider);
};

struct SSLSocketDataProvider {
  SSLSocketDataProvider(bool async, int result);
  ~SSLSocketDataProvider();

  MockConnect connect;
  SSLClientSocket::NextProtoStatus next_proto_status;
  std::string next_proto;
  bool was_npn_negotiated;
  net::SSLCertRequestInfo* cert_request_info;
  scoped_refptr<X509Certificate> cert_;
};

class DelayedSocketData : public StaticSocketDataProvider,
                          public base::RefCounted<DelayedSocketData> {
 public:
  
  
  
  
  
  
  
  DelayedSocketData(int write_delay,
                    MockRead* reads, size_t reads_count,
                    MockWrite* writes, size_t writes_count);

  
  
  
  
  
  
  
  
  DelayedSocketData(const MockConnect& connect, int write_delay,
                    MockRead* reads, size_t reads_count,
                    MockWrite* writes, size_t writes_count);
  ~DelayedSocketData();

  void ForceNextRead();

  
  virtual MockRead GetNextRead();
  virtual MockWriteResult OnWrite(const std::string& data);
  virtual void Reset();
  virtual void CompleteRead();

 private:
  int write_delay_;
  ScopedRunnableMethodFactory<DelayedSocketData> factory_;
};

class OrderedSocketData : public StaticSocketDataProvider,
                          public base::RefCounted<OrderedSocketData> {
 public:
  
  
  
  
  
  OrderedSocketData(MockRead* reads, size_t reads_count,
                    MockWrite* writes, size_t writes_count);

  
  
  
  
  
  
  OrderedSocketData(const MockConnect& connect,
                    MockRead* reads, size_t reads_count,
                    MockWrite* writes, size_t writes_count);

  void SetCompletionCallback(CompletionCallback* callback) {
    callback_ = callback;
  }

  
  void EndLoop();

  
  virtual MockRead GetNextRead();
  virtual MockWriteResult OnWrite(const std::string& data);
  virtual void Reset();
  virtual void CompleteRead();

 private:
  friend class base::RefCounted<OrderedSocketData>;
  virtual ~OrderedSocketData();

  int sequence_number_;
  int loop_stop_stage_;
  CompletionCallback* callback_;
  bool blocked_;
  ScopedRunnableMethodFactory<OrderedSocketData> factory_;
};

class DeterministicMockTCPClientSocket;

class DeterministicSocketData : public StaticSocketDataProvider,
    public base::RefCounted<DeterministicSocketData> {
 public:
  
  
  DeterministicSocketData(MockRead* reads, size_t reads_count,
                          MockWrite* writes, size_t writes_count);
  virtual ~DeterministicSocketData();

  
  void Run();

  
  void RunFor(int steps);

  
  virtual void SetStop(int seq);

  
  virtual void StopAfter(int seq);
  bool stopped() const { return stopped_; }
  void SetStopped(bool val) { stopped_ = val; }
  MockRead& current_read() { return current_read_; }
  MockRead& current_write() { return current_write_; }
  int sequence_number() const { return sequence_number_; }
  void set_socket(base::WeakPtr<DeterministicMockTCPClientSocket> socket) {
    socket_ = socket;
  }

  

  
  
  virtual MockRead GetNextRead();

  
  // checks to make sure the written data matches the expected data. The
  
  virtual MockWriteResult OnWrite(const std::string& data);
  virtual void Reset();
  virtual void CompleteRead() {}

 private:
  
  void InvokeCallbacks();

  void NextStep();

  int sequence_number_;
  MockRead current_read_;
  MockWrite current_write_;
  int stopping_sequence_number_;
  bool stopped_;
  base::WeakPtr<DeterministicMockTCPClientSocket> socket_;
  bool print_debug_;
};

template<typename T>
class SocketDataProviderArray {
 public:
  SocketDataProviderArray() : next_index_(0) {
  }

  T* GetNext() {
    DCHECK_LT(next_index_, data_providers_.size());
    return data_providers_[next_index_++];
  }

  void Add(T* data_provider) {
    DCHECK(data_provider);
    data_providers_.push_back(data_provider);
  }

  void ResetNextIndex() {
    next_index_ = 0;
  }

 private:
  
  
  size_t next_index_;

  
  std::vector<T*> data_providers_;
};

class MockTCPClientSocket;
class MockSSLClientSocket;

class MockClientSocketFactory : public ClientSocketFactory {
 public:
  MockClientSocketFactory();
  virtual ~MockClientSocketFactory();

  void AddSocketDataProvider(SocketDataProvider* socket);
  void AddSSLSocketDataProvider(SSLSocketDataProvider* socket);
  void ResetNextMockIndexes();

  
  
  MockTCPClientSocket* GetMockTCPClientSocket(size_t index) const;

  
  
  MockSSLClientSocket* GetMockSSLClientSocket(size_t index) const;

  SocketDataProviderArray<SocketDataProvider>& mock_data() {
    return mock_data_;
  }
  std::vector<MockTCPClientSocket*>& tcp_client_sockets() {
    return tcp_client_sockets_;
  }

  
  virtual ClientSocket* CreateTransportClientSocket(
      const AddressList& addresses,
      NetLog* net_log,
      const NetLog::Source& source);
  virtual SSLClientSocket* CreateSSLClientSocket(
      ClientSocketHandle* transport_socket,
      const HostPortPair& host_and_port,
      const SSLConfig& ssl_config,
      SSLHostInfo* ssl_host_info,
      CertVerifier* cert_verifier,
      DnsCertProvenanceChecker* dns_cert_checker);
  virtual void ClearSSLSessionCache();

 private:
  SocketDataProviderArray<SocketDataProvider> mock_data_;
  SocketDataProviderArray<SSLSocketDataProvider> mock_ssl_data_;

  
  std::vector<MockTCPClientSocket*> tcp_client_sockets_;
  std::vector<MockSSLClientSocket*> ssl_client_sockets_;
};

class MockClientSocket : public net::SSLClientSocket {
 public:
  explicit MockClientSocket(net::NetLog* net_log);

  
  
  
  
  
  virtual void OnReadComplete(const MockRead& data) = 0;

  
  virtual int Read(net::IOBuffer* buf, int buf_len,
                   net::CompletionCallback* callback) = 0;
  virtual int Write(net::IOBuffer* buf, int buf_len,
                    net::CompletionCallback* callback) = 0;
  virtual bool SetReceiveBufferSize(int32 size);
  virtual bool SetSendBufferSize(int32 size);

  
  virtual int Connect(net::CompletionCallback* callback) = 0;
  virtual void Disconnect();
  virtual bool IsConnected() const;
  virtual bool IsConnectedAndIdle() const;
  virtual int GetPeerAddress(AddressList* address) const;
  virtual int GetLocalAddress(IPEndPoint* address) const;
  virtual const BoundNetLog& NetLog() const;
  virtual void SetSubresourceSpeculation() {}
  virtual void SetOmniboxSpeculation() {}

  
  virtual void GetSSLInfo(net::SSLInfo* ssl_info);
  virtual void GetSSLCertRequestInfo(
      net::SSLCertRequestInfo* cert_request_info);
  virtual NextProtoStatus GetNextProto(std::string* proto);

 protected:
  virtual ~MockClientSocket();
  void RunCallbackAsync(net::CompletionCallback* callback, int result);
  void RunCallback(net::CompletionCallback*, int result);

  ScopedRunnableMethodFactory<MockClientSocket> method_factory_;

  
  bool connected_;

  net::BoundNetLog net_log_;
};

class MockTCPClientSocket : public MockClientSocket {
 public:
  MockTCPClientSocket(const net::AddressList& addresses, net::NetLog* net_log,
                      net::SocketDataProvider* socket);

  net::AddressList addresses() const { return addresses_; }

  
  virtual int Read(net::IOBuffer* buf, int buf_len,
                   net::CompletionCallback* callback);
  virtual int Write(net::IOBuffer* buf, int buf_len,
                    net::CompletionCallback* callback);

  
  virtual int Connect(net::CompletionCallback* callback);
  virtual void Disconnect();
  virtual bool IsConnected() const;
  virtual bool IsConnectedAndIdle() const;
  virtual int GetPeerAddress(AddressList* address) const;
  virtual bool WasEverUsed() const;
  virtual bool UsingTCPFastOpen() const;

  
  virtual void OnReadComplete(const MockRead& data);

 private:
  int CompleteRead();

  net::AddressList addresses_;

  net::SocketDataProvider* data_;
  int read_offset_;
  net::MockRead read_data_;
  bool need_read_data_;

  
  
  
  bool peer_closed_connection_;

  
  net::IOBuffer* pending_buf_;
  int pending_buf_len_;
  net::CompletionCallback* pending_callback_;
  bool was_used_to_convey_data_;
};

class DeterministicMockTCPClientSocket : public MockClientSocket,
    public base::SupportsWeakPtr<DeterministicMockTCPClientSocket> {
 public:
  DeterministicMockTCPClientSocket(net::NetLog* net_log,
      net::DeterministicSocketData* data);
  virtual ~DeterministicMockTCPClientSocket();

  bool write_pending() const { return write_pending_; }
  bool read_pending() const { return read_pending_; }

  void CompleteWrite();
  int CompleteRead();

  
  virtual int Write(net::IOBuffer* buf, int buf_len,
                    net::CompletionCallback* callback);
  virtual int Read(net::IOBuffer* buf, int buf_len,
                   net::CompletionCallback* callback);

  
  virtual int Connect(net::CompletionCallback* callback);
  virtual void Disconnect();
  virtual bool IsConnected() const;
  virtual bool IsConnectedAndIdle() const;
  virtual bool WasEverUsed() const;
  virtual bool UsingTCPFastOpen() const;

  
  virtual void OnReadComplete(const MockRead& data);

 private:
  bool write_pending_;
  net::CompletionCallback* write_callback_;
  int write_result_;

  net::MockRead read_data_;

  net::IOBuffer* read_buf_;
  int read_buf_len_;
  bool read_pending_;
  net::CompletionCallback* read_callback_;
  net::DeterministicSocketData* data_;
  bool was_used_to_convey_data_;
};

class MockSSLClientSocket : public MockClientSocket {
 public:
  MockSSLClientSocket(
      net::ClientSocketHandle* transport_socket,
      const HostPortPair& host_and_port,
      const net::SSLConfig& ssl_config,
      SSLHostInfo* ssl_host_info,
      net::SSLSocketDataProvider* socket);
  virtual ~MockSSLClientSocket();

  
  virtual int Read(net::IOBuffer* buf, int buf_len,
                   net::CompletionCallback* callback);
  virtual int Write(net::IOBuffer* buf, int buf_len,
                    net::CompletionCallback* callback);

  
  virtual int Connect(net::CompletionCallback* callback);
  virtual void Disconnect();
  virtual bool IsConnected() const;
  virtual bool WasEverUsed() const;
  virtual bool UsingTCPFastOpen() const;

  
  virtual void GetSSLInfo(net::SSLInfo* ssl_info);
  virtual void GetSSLCertRequestInfo(
      net::SSLCertRequestInfo* cert_request_info);
  virtual NextProtoStatus GetNextProto(std::string* proto);
  virtual bool was_npn_negotiated() const;
  virtual bool set_was_npn_negotiated(bool negotiated);

  
  virtual void OnReadComplete(const MockRead& data);

 private:
  class ConnectCallback;

  scoped_ptr<ClientSocketHandle> transport_;
  net::SSLSocketDataProvider* data_;
  bool is_npn_state_set_;
  bool new_npn_value_;
  bool was_used_to_convey_data_;
};

class TestSocketRequest : public CallbackRunner< Tuple1<int> > {
 public:
  TestSocketRequest(
      std::vector<TestSocketRequest*>* request_order,
      size_t* completion_count);
  virtual ~TestSocketRequest();

  ClientSocketHandle* handle() { return &handle_; }

  int WaitForResult();
  virtual void RunWithParams(const Tuple1<int>& params);

 private:
  ClientSocketHandle handle_;
  std::vector<TestSocketRequest*>* request_order_;
  size_t* completion_count_;
  TestCompletionCallback callback_;
};

class ClientSocketPoolTest {
 public:
  enum KeepAlive {
    KEEP_ALIVE,

    
    NO_KEEP_ALIVE,
  };

  static const int kIndexOutOfBounds;
  static const int kRequestNotFound;

  ClientSocketPoolTest();
  ~ClientSocketPoolTest();

  template <typename PoolType, typename SocketParams>
  int StartRequestUsingPool(PoolType* socket_pool,
                            const std::string& group_name,
                            RequestPriority priority,
                            const scoped_refptr<SocketParams>& socket_params) {
    DCHECK(socket_pool);
    TestSocketRequest* request = new TestSocketRequest(&request_order_,
                                                       &completion_count_);
    requests_.push_back(request);
    int rv = request->handle()->Init(
        group_name, socket_params, priority, request,
        socket_pool, BoundNetLog());
    if (rv != ERR_IO_PENDING)
      request_order_.push_back(request);
    return rv;
  }

  
  
  
  
  int GetOrderOfRequest(size_t index) const;

  
  
  bool ReleaseOneConnection(KeepAlive keep_alive);

  
  void ReleaseAllConnections(KeepAlive keep_alive);

  TestSocketRequest* request(int i) { return requests_[i]; }
  size_t requests_size() const { return requests_.size(); }
  ScopedVector<TestSocketRequest>* requests() { return &requests_; }
  size_t completion_count() const { return completion_count_; }

 private:
  ScopedVector<TestSocketRequest> requests_;
  std::vector<TestSocketRequest*> request_order_;
  size_t completion_count_;
};

class MockTransportClientSocketPool : public TransportClientSocketPool {
 public:
  class MockConnectJob {
   public:
    MockConnectJob(ClientSocket* socket, ClientSocketHandle* handle,
                   CompletionCallback* callback);
    ~MockConnectJob();

    int Connect();
    bool CancelHandle(const ClientSocketHandle* handle);

   private:
    void OnConnect(int rv);

    scoped_ptr<ClientSocket> socket_;
    ClientSocketHandle* handle_;
    CompletionCallback* user_callback_;
    CompletionCallbackImpl<MockConnectJob> connect_callback_;

    DISALLOW_COPY_AND_ASSIGN(MockConnectJob);
  };

  MockTransportClientSocketPool(
      int max_sockets,
      int max_sockets_per_group,
      ClientSocketPoolHistograms* histograms,
      ClientSocketFactory* socket_factory);

  virtual ~MockTransportClientSocketPool();

  int release_count() const { return release_count_; }
  int cancel_count() const { return cancel_count_; }

  
  virtual int RequestSocket(const std::string& group_name,
                            const void* socket_params,
                            RequestPriority priority,
                            ClientSocketHandle* handle,
                            CompletionCallback* callback,
                            const BoundNetLog& net_log);

  virtual void CancelRequest(const std::string& group_name,
                             ClientSocketHandle* handle);
  virtual void ReleaseSocket(const std::string& group_name,
                             ClientSocket* socket, int id);

 private:
  ClientSocketFactory* client_socket_factory_;
  ScopedVector<MockConnectJob> job_list_;
  int release_count_;
  int cancel_count_;

  DISALLOW_COPY_AND_ASSIGN(MockTransportClientSocketPool);
};

class DeterministicMockClientSocketFactory : public ClientSocketFactory {
 public:
  DeterministicMockClientSocketFactory();
  virtual ~DeterministicMockClientSocketFactory();

  void AddSocketDataProvider(DeterministicSocketData* socket);
  void AddSSLSocketDataProvider(SSLSocketDataProvider* socket);
  void ResetNextMockIndexes();

  
  
  MockSSLClientSocket* GetMockSSLClientSocket(size_t index) const;

  SocketDataProviderArray<DeterministicSocketData>& mock_data() {
    return mock_data_;
  }
  std::vector<DeterministicMockTCPClientSocket*>& tcp_client_sockets() {
    return tcp_client_sockets_;
  }

  
  virtual ClientSocket* CreateTransportClientSocket(
      const AddressList& addresses,
      NetLog* net_log,
      const NetLog::Source& source);
  virtual SSLClientSocket* CreateSSLClientSocket(
      ClientSocketHandle* transport_socket,
      const HostPortPair& host_and_port,
      const SSLConfig& ssl_config,
      SSLHostInfo* ssl_host_info,
      CertVerifier* cert_verifier,
      DnsCertProvenanceChecker* dns_cert_checker);
  virtual void ClearSSLSessionCache();

 private:
  SocketDataProviderArray<DeterministicSocketData> mock_data_;
  SocketDataProviderArray<SSLSocketDataProvider> mock_ssl_data_;

  
  std::vector<DeterministicMockTCPClientSocket*> tcp_client_sockets_;
  std::vector<MockSSLClientSocket*> ssl_client_sockets_;
};

class MockSOCKSClientSocketPool : public SOCKSClientSocketPool {
 public:
  MockSOCKSClientSocketPool(
      int max_sockets,
      int max_sockets_per_group,
      ClientSocketPoolHistograms* histograms,
      TransportClientSocketPool* transport_pool);

  virtual ~MockSOCKSClientSocketPool();

  
  virtual int RequestSocket(const std::string& group_name,
                            const void* socket_params,
                            RequestPriority priority,
                            ClientSocketHandle* handle,
                            CompletionCallback* callback,
                            const BoundNetLog& net_log);

  virtual void CancelRequest(const std::string& group_name,
                             ClientSocketHandle* handle);
  virtual void ReleaseSocket(const std::string& group_name,
                             ClientSocket* socket, int id);

 private:
  TransportClientSocketPool* const transport_pool_;

  DISALLOW_COPY_AND_ASSIGN(MockSOCKSClientSocketPool);
};

extern const char kSOCKS5GreetRequest[];
extern const int kSOCKS5GreetRequestLength;

extern const char kSOCKS5GreetResponse[];
extern const int kSOCKS5GreetResponseLength;

extern const char kSOCKS5OkRequest[];
extern const int kSOCKS5OkRequestLength;

extern const char kSOCKS5OkResponse[];
extern const int kSOCKS5OkResponseLength;

}  

#endif  
