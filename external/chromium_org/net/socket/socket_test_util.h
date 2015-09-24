// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SOCKET_TEST_UTIL_H_
#define NET_SOCKET_SOCKET_TEST_UTIL_H_

#include <cstring>
#include <deque>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "net/base/address_list.h"
#include "net/base/io_buffer.h"
#include "net/base/net_errors.h"
#include "net/base/net_log.h"
#include "net/base/test_completion_callback.h"
#include "net/http/http_auth_controller.h"
#include "net/http/http_proxy_client_socket_pool.h"
#include "net/socket/client_socket_factory.h"
#include "net/socket/client_socket_handle.h"
#include "net/socket/socks_client_socket_pool.h"
#include "net/socket/ssl_client_socket.h"
#include "net/socket/ssl_client_socket_pool.h"
#include "net/socket/transport_client_socket_pool.h"
#include "net/ssl/ssl_config_service.h"
#include "net/udp/datagram_client_socket.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace net {

enum {
  
  
  
  
  
  ERR_TEST_PEER_CLOSE_AFTER_NEXT_MOCK_READ = -10000,
};

class AsyncSocket;
class MockClientSocket;
class ServerBoundCertService;
class SSLClientSocket;
class StreamSocket;

enum IoMode {
  ASYNC,
  SYNCHRONOUS
};

struct MockConnect {
  
  
  
  MockConnect();
  
  
  MockConnect(IoMode io_mode, int r);
  MockConnect(IoMode io_mode, int r, IPEndPoint addr);
  ~MockConnect();

  IoMode mode;
  int result;
  IPEndPoint peer_addr;
};

enum MockReadWriteType {
  MOCK_READ,
  MOCK_WRITE
};

template <MockReadWriteType type>
struct MockReadWrite {
  
  enum {
    STOPLOOP = 1 << 31
  };

  
  MockReadWrite()
      : mode(SYNCHRONOUS),
        result(0),
        data(NULL),
        data_len(0),
        sequence_number(0),
        time_stamp(base::Time::Now()) {}

  
  MockReadWrite(IoMode io_mode, int result)
      : mode(io_mode),
        result(result),
        data(NULL),
        data_len(0),
        sequence_number(0),
        time_stamp(base::Time::Now()) {}

  
  MockReadWrite(IoMode io_mode, int result, int seq)
      : mode(io_mode),
        result(result),
        data(NULL),
        data_len(0),
        sequence_number(seq),
        time_stamp(base::Time::Now()) {}

  
  explicit MockReadWrite(const char* data)
      : mode(ASYNC),
        result(0),
        data(data),
        data_len(strlen(data)),
        sequence_number(0),
        time_stamp(base::Time::Now()) {}

  
  MockReadWrite(IoMode io_mode, const char* data)
      : mode(io_mode),
        result(0),
        data(data),
        data_len(strlen(data)),
        sequence_number(0),
        time_stamp(base::Time::Now()) {}

  
  MockReadWrite(IoMode io_mode, const char* data, int data_len)
      : mode(io_mode),
        result(0),
        data(data),
        data_len(data_len),
        sequence_number(0),
        time_stamp(base::Time::Now()) {}

  
  MockReadWrite(IoMode io_mode, int seq, const char* data)
      : mode(io_mode),
        result(0),
        data(data),
        data_len(strlen(data)),
        sequence_number(seq),
        time_stamp(base::Time::Now()) {}

  
  MockReadWrite(IoMode io_mode, const char* data, int data_len, int seq)
      : mode(io_mode),
        result(0),
        data(data),
        data_len(data_len),
        sequence_number(seq),
        time_stamp(base::Time::Now()) {}

  IoMode mode;
  int result;
  const char* data;
  int data_len;

  
  
  
  int sequence_number;    
                          
  base::Time time_stamp;  
};

typedef MockReadWrite<MOCK_READ> MockRead;
typedef MockReadWrite<MOCK_WRITE> MockWrite;

struct MockWriteResult {
  MockWriteResult(IoMode io_mode, int result) : mode(io_mode), result(result) {}

  IoMode mode;
  int result;
};

class SocketDataProvider {
 public:
  SocketDataProvider() : socket_(NULL) {}

  virtual ~SocketDataProvider() {}

  
  
  
  
  virtual MockRead GetNextRead() = 0;
  virtual MockWriteResult OnWrite(const std::string& data) = 0;
  virtual void Reset() = 0;

  
  AsyncSocket* socket() { return socket_; }
  void set_socket(AsyncSocket* socket) { socket_ = socket; }

  MockConnect connect_data() const { return connect_; }
  void set_connect_data(const MockConnect& connect) { connect_ = connect; }

 private:
  MockConnect connect_;
  AsyncSocket* socket_;

  DISALLOW_COPY_AND_ASSIGN(SocketDataProvider);
};

class AsyncSocket {
 public:
  
  
  
  
  
  virtual void OnReadComplete(const MockRead& data) = 0;
  virtual void OnConnectComplete(const MockConnect& data) = 0;
};

class StaticSocketDataProvider : public SocketDataProvider {
 public:
  StaticSocketDataProvider();
  StaticSocketDataProvider(MockRead* reads,
                           size_t reads_count,
                           MockWrite* writes,
                           size_t writes_count);
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

  
  virtual MockRead GetNextRead() OVERRIDE;
  virtual MockWriteResult OnWrite(const std::string& data) OVERRIDE;
  virtual void Reset() OVERRIDE;

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

  
  virtual MockRead GetNextRead() OVERRIDE;
  virtual MockWriteResult OnWrite(const std::string& data) = 0;
  virtual void Reset() OVERRIDE;

 protected:
  
  
  void SimulateRead(const char* data, size_t length);
  void SimulateRead(const char* data) { SimulateRead(data, std::strlen(data)); }

 private:
  std::deque<MockRead> reads_;

  
  int short_read_limit_;

  
  
  bool allow_unconsumed_reads_;

  DISALLOW_COPY_AND_ASSIGN(DynamicSocketDataProvider);
};

struct SSLSocketDataProvider {
  SSLSocketDataProvider(IoMode mode, int result);
  ~SSLSocketDataProvider();

  void SetNextProto(NextProto proto);

  MockConnect connect;
  SSLClientSocket::NextProtoStatus next_proto_status;
  std::string next_proto;
  std::string server_protos;
  bool was_npn_negotiated;
  NextProto protocol_negotiated;
  bool client_cert_sent;
  SSLCertRequestInfo* cert_request_info;
  scoped_refptr<X509Certificate> cert;
  bool channel_id_sent;
  ServerBoundCertService* server_bound_cert_service;
};

class DelayedSocketData : public StaticSocketDataProvider {
 public:
  
  
  
  
  
  
  DelayedSocketData(int write_delay,
                    MockRead* reads,
                    size_t reads_count,
                    MockWrite* writes,
                    size_t writes_count);

  
  
  
  
  
  
  
  DelayedSocketData(const MockConnect& connect,
                    int write_delay,
                    MockRead* reads,
                    size_t reads_count,
                    MockWrite* writes,
                    size_t writes_count);
  virtual ~DelayedSocketData();

  void ForceNextRead();

  
  virtual MockRead GetNextRead() OVERRIDE;
  virtual MockWriteResult OnWrite(const std::string& data) OVERRIDE;
  virtual void Reset() OVERRIDE;
  virtual void CompleteRead() OVERRIDE;

 private:
  int write_delay_;
  bool read_in_progress_;

  base::WeakPtrFactory<DelayedSocketData> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(DelayedSocketData);
};

class OrderedSocketData : public StaticSocketDataProvider {
 public:
  
  
  
  
  
  OrderedSocketData(MockRead* reads,
                    size_t reads_count,
                    MockWrite* writes,
                    size_t writes_count);
  virtual ~OrderedSocketData();

  
  
  
  
  
  
  OrderedSocketData(const MockConnect& connect,
                    MockRead* reads,
                    size_t reads_count,
                    MockWrite* writes,
                    size_t writes_count);

  
  void EndLoop();

  
  virtual MockRead GetNextRead() OVERRIDE;
  virtual MockWriteResult OnWrite(const std::string& data) OVERRIDE;
  virtual void Reset() OVERRIDE;
  virtual void CompleteRead() OVERRIDE;

 private:
  int sequence_number_;
  int loop_stop_stage_;
  bool blocked_;

  base::WeakPtrFactory<OrderedSocketData> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(OrderedSocketData);
};

class DeterministicMockTCPClientSocket;

class DeterministicSocketData : public StaticSocketDataProvider {
 public:
  
  
  
  
  
  class Delegate {
   public:
    
    
    
    virtual bool WritePending() const = 0;
    
    
    
    virtual bool ReadPending() const = 0;
    
    virtual void CompleteWrite() = 0;
    
    virtual int CompleteRead() = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  DeterministicSocketData(MockRead* reads,
                          size_t reads_count,
                          MockWrite* writes,
                          size_t writes_count);
  virtual ~DeterministicSocketData();

  
  void Run();

  
  void RunFor(int steps);

  
  virtual void SetStop(int seq);

  
  virtual void StopAfter(int seq);
  bool stopped() const { return stopped_; }
  void SetStopped(bool val) { stopped_ = val; }
  MockRead& current_read() { return current_read_; }
  MockWrite& current_write() { return current_write_; }
  int sequence_number() const { return sequence_number_; }
  void set_delegate(base::WeakPtr<Delegate> delegate) { delegate_ = delegate; }

  

  
  
  virtual MockRead GetNextRead() OVERRIDE;

  
  // checks to make sure the written data matches the expected data. The
  
  virtual MockWriteResult OnWrite(const std::string& data) OVERRIDE;
  virtual void Reset() OVERRIDE;
  virtual void CompleteRead() OVERRIDE {}

 private:
  
  void InvokeCallbacks();

  void NextStep();

  void VerifyCorrectSequenceNumbers(MockRead* reads,
                                    size_t reads_count,
                                    MockWrite* writes,
                                    size_t writes_count);

  int sequence_number_;
  MockRead current_read_;
  MockWrite current_write_;
  int stopping_sequence_number_;
  bool stopped_;
  base::WeakPtr<Delegate> delegate_;
  bool print_debug_;
  bool is_running_;
};

template <typename T>
class SocketDataProviderArray {
 public:
  SocketDataProviderArray() : next_index_(0) {}

  T* GetNext() {
    DCHECK_LT(next_index_, data_providers_.size());
    return data_providers_[next_index_++];
  }

  void Add(T* data_provider) {
    DCHECK(data_provider);
    data_providers_.push_back(data_provider);
  }

  size_t next_index() { return next_index_; }

  void ResetNextIndex() { next_index_ = 0; }

 private:
  
  
  size_t next_index_;

  
  std::vector<T*> data_providers_;
};

class MockUDPClientSocket;
class MockTCPClientSocket;
class MockSSLClientSocket;

class MockClientSocketFactory : public ClientSocketFactory {
 public:
  MockClientSocketFactory();
  virtual ~MockClientSocketFactory();

  void AddSocketDataProvider(SocketDataProvider* socket);
  void AddSSLSocketDataProvider(SSLSocketDataProvider* socket);
  void ResetNextMockIndexes();

  SocketDataProviderArray<SocketDataProvider>& mock_data() {
    return mock_data_;
  }

  
  virtual scoped_ptr<DatagramClientSocket> CreateDatagramClientSocket(
      DatagramSocket::BindType bind_type,
      const RandIntCallback& rand_int_cb,
      NetLog* net_log,
      const NetLog::Source& source) OVERRIDE;
  virtual scoped_ptr<StreamSocket> CreateTransportClientSocket(
      const AddressList& addresses,
      NetLog* net_log,
      const NetLog::Source& source) OVERRIDE;
  virtual scoped_ptr<SSLClientSocket> CreateSSLClientSocket(
      scoped_ptr<ClientSocketHandle> transport_socket,
      const HostPortPair& host_and_port,
      const SSLConfig& ssl_config,
      const SSLClientSocketContext& context) OVERRIDE;
  virtual void ClearSSLSessionCache() OVERRIDE;

 private:
  SocketDataProviderArray<SocketDataProvider> mock_data_;
  SocketDataProviderArray<SSLSocketDataProvider> mock_ssl_data_;
};

class MockClientSocket : public SSLClientSocket {
 public:
  
  static const char kTlsUnique[];

  
  
  explicit MockClientSocket(const BoundNetLog& net_log);

  
  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   const CompletionCallback& callback) = 0;
  virtual int Write(IOBuffer* buf,
                    int buf_len,
                    const CompletionCallback& callback) = 0;
  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;

  
  virtual int Connect(const CompletionCallback& callback) = 0;
  virtual void Disconnect() OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool IsConnectedAndIdle() const OVERRIDE;
  virtual int GetPeerAddress(IPEndPoint* address) const OVERRIDE;
  virtual int GetLocalAddress(IPEndPoint* address) const OVERRIDE;
  virtual const BoundNetLog& NetLog() const OVERRIDE;
  virtual void SetSubresourceSpeculation() OVERRIDE {}
  virtual void SetOmniboxSpeculation() OVERRIDE {}

  
  virtual void GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info)
      OVERRIDE;
  virtual int ExportKeyingMaterial(const base::StringPiece& label,
                                   bool has_context,
                                   const base::StringPiece& context,
                                   unsigned char* out,
                                   unsigned int outlen) OVERRIDE;
  virtual int GetTLSUniqueChannelBinding(std::string* out) OVERRIDE;
  virtual NextProtoStatus GetNextProto(std::string* proto,
                                       std::string* server_protos) OVERRIDE;
  virtual ServerBoundCertService* GetServerBoundCertService() const OVERRIDE;

 protected:
  virtual ~MockClientSocket();
  void RunCallbackAsync(const CompletionCallback& callback, int result);
  void RunCallback(const CompletionCallback& callback, int result);

  
  bool connected_;

  
  IPEndPoint peer_addr_;

  BoundNetLog net_log_;

  base::WeakPtrFactory<MockClientSocket> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(MockClientSocket);
};

class MockTCPClientSocket : public MockClientSocket, public AsyncSocket {
 public:
  MockTCPClientSocket(const AddressList& addresses,
                      net::NetLog* net_log,
                      SocketDataProvider* socket);
  virtual ~MockTCPClientSocket();

  const AddressList& addresses() const { return addresses_; }

  
  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   const CompletionCallback& callback) OVERRIDE;
  virtual int Write(IOBuffer* buf,
                    int buf_len,
                    const CompletionCallback& callback) OVERRIDE;

  
  virtual int Connect(const CompletionCallback& callback) OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool IsConnectedAndIdle() const OVERRIDE;
  virtual int GetPeerAddress(IPEndPoint* address) const OVERRIDE;
  virtual bool WasEverUsed() const OVERRIDE;
  virtual bool UsingTCPFastOpen() const OVERRIDE;
  virtual bool WasNpnNegotiated() const OVERRIDE;
  virtual bool GetSSLInfo(SSLInfo* ssl_info) OVERRIDE;

  
  virtual void OnReadComplete(const MockRead& data) OVERRIDE;
  virtual void OnConnectComplete(const MockConnect& data) OVERRIDE;

 private:
  int CompleteRead();

  AddressList addresses_;

  SocketDataProvider* data_;
  int read_offset_;
  MockRead read_data_;
  bool need_read_data_;

  
  
  
  bool peer_closed_connection_;

  
  scoped_refptr<IOBuffer> pending_buf_;
  int pending_buf_len_;
  CompletionCallback pending_callback_;
  bool was_used_to_convey_data_;

  DISALLOW_COPY_AND_ASSIGN(MockTCPClientSocket);
};

class DeterministicSocketHelper {
 public:
  DeterministicSocketHelper(net::NetLog* net_log,
                            DeterministicSocketData* data);
  virtual ~DeterministicSocketHelper();

  bool write_pending() const { return write_pending_; }
  bool read_pending() const { return read_pending_; }

  void CompleteWrite();
  int CompleteRead();

  int Write(IOBuffer* buf, int buf_len, const CompletionCallback& callback);
  int Read(IOBuffer* buf, int buf_len, const CompletionCallback& callback);

  const BoundNetLog& net_log() const { return net_log_; }

  bool was_used_to_convey_data() const { return was_used_to_convey_data_; }

  bool peer_closed_connection() const { return peer_closed_connection_; }

  DeterministicSocketData* data() const { return data_; }

 private:
  bool write_pending_;
  CompletionCallback write_callback_;
  int write_result_;

  MockRead read_data_;

  IOBuffer* read_buf_;
  int read_buf_len_;
  bool read_pending_;
  CompletionCallback read_callback_;
  DeterministicSocketData* data_;
  bool was_used_to_convey_data_;
  bool peer_closed_connection_;
  BoundNetLog net_log_;
};

class DeterministicMockUDPClientSocket
    : public DatagramClientSocket,
      public AsyncSocket,
      public DeterministicSocketData::Delegate,
      public base::SupportsWeakPtr<DeterministicMockUDPClientSocket> {
 public:
  DeterministicMockUDPClientSocket(net::NetLog* net_log,
                                   DeterministicSocketData* data);
  virtual ~DeterministicMockUDPClientSocket();

  
  virtual bool WritePending() const OVERRIDE;
  virtual bool ReadPending() const OVERRIDE;
  virtual void CompleteWrite() OVERRIDE;
  virtual int CompleteRead() OVERRIDE;

  
  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   const CompletionCallback& callback) OVERRIDE;
  virtual int Write(IOBuffer* buf,
                    int buf_len,
                    const CompletionCallback& callback) OVERRIDE;
  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;

  
  virtual void Close() OVERRIDE;
  virtual int GetPeerAddress(IPEndPoint* address) const OVERRIDE;
  virtual int GetLocalAddress(IPEndPoint* address) const OVERRIDE;
  virtual const BoundNetLog& NetLog() const OVERRIDE;

  
  virtual int Connect(const IPEndPoint& address) OVERRIDE;

  
  virtual void OnReadComplete(const MockRead& data) OVERRIDE;
  virtual void OnConnectComplete(const MockConnect& data) OVERRIDE;

 private:
  bool connected_;
  IPEndPoint peer_address_;
  DeterministicSocketHelper helper_;

  DISALLOW_COPY_AND_ASSIGN(DeterministicMockUDPClientSocket);
};

class DeterministicMockTCPClientSocket
    : public MockClientSocket,
      public AsyncSocket,
      public DeterministicSocketData::Delegate,
      public base::SupportsWeakPtr<DeterministicMockTCPClientSocket> {
 public:
  DeterministicMockTCPClientSocket(net::NetLog* net_log,
                                   DeterministicSocketData* data);
  virtual ~DeterministicMockTCPClientSocket();

  
  virtual bool WritePending() const OVERRIDE;
  virtual bool ReadPending() const OVERRIDE;
  virtual void CompleteWrite() OVERRIDE;
  virtual int CompleteRead() OVERRIDE;

  
  virtual int Write(IOBuffer* buf,
                    int buf_len,
                    const CompletionCallback& callback) OVERRIDE;
  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   const CompletionCallback& callback) OVERRIDE;

  
  virtual int Connect(const CompletionCallback& callback) OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool IsConnectedAndIdle() const OVERRIDE;
  virtual bool WasEverUsed() const OVERRIDE;
  virtual bool UsingTCPFastOpen() const OVERRIDE;
  virtual bool WasNpnNegotiated() const OVERRIDE;
  virtual bool GetSSLInfo(SSLInfo* ssl_info) OVERRIDE;

  
  virtual void OnReadComplete(const MockRead& data) OVERRIDE;
  virtual void OnConnectComplete(const MockConnect& data) OVERRIDE;

 private:
  DeterministicSocketHelper helper_;

  DISALLOW_COPY_AND_ASSIGN(DeterministicMockTCPClientSocket);
};

class MockSSLClientSocket : public MockClientSocket, public AsyncSocket {
 public:
  MockSSLClientSocket(scoped_ptr<ClientSocketHandle> transport_socket,
                      const HostPortPair& host_and_port,
                      const SSLConfig& ssl_config,
                      SSLSocketDataProvider* socket);
  virtual ~MockSSLClientSocket();

  
  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   const CompletionCallback& callback) OVERRIDE;
  virtual int Write(IOBuffer* buf,
                    int buf_len,
                    const CompletionCallback& callback) OVERRIDE;

  
  virtual int Connect(const CompletionCallback& callback) OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool WasEverUsed() const OVERRIDE;
  virtual bool UsingTCPFastOpen() const OVERRIDE;
  virtual int GetPeerAddress(IPEndPoint* address) const OVERRIDE;
  virtual bool WasNpnNegotiated() const OVERRIDE;
  virtual bool GetSSLInfo(SSLInfo* ssl_info) OVERRIDE;

  
  virtual void GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info)
      OVERRIDE;
  virtual NextProtoStatus GetNextProto(std::string* proto,
                                       std::string* server_protos) OVERRIDE;
  virtual bool set_was_npn_negotiated(bool negotiated) OVERRIDE;
  virtual void set_protocol_negotiated(NextProto protocol_negotiated) OVERRIDE;
  virtual NextProto GetNegotiatedProtocol() const OVERRIDE;

  
  virtual void OnReadComplete(const MockRead& data) OVERRIDE;
  virtual void OnConnectComplete(const MockConnect& data) OVERRIDE;

  virtual bool WasChannelIDSent() const OVERRIDE;
  virtual void set_channel_id_sent(bool channel_id_sent) OVERRIDE;
  virtual ServerBoundCertService* GetServerBoundCertService() const OVERRIDE;

 private:
  static void ConnectCallback(MockSSLClientSocket* ssl_client_socket,
                              const CompletionCallback& callback,
                              int rv);

  scoped_ptr<ClientSocketHandle> transport_;
  SSLSocketDataProvider* data_;
  bool is_npn_state_set_;
  bool new_npn_value_;
  bool is_protocol_negotiated_set_;
  NextProto protocol_negotiated_;

  DISALLOW_COPY_AND_ASSIGN(MockSSLClientSocket);
};

class MockUDPClientSocket : public DatagramClientSocket, public AsyncSocket {
 public:
  MockUDPClientSocket(SocketDataProvider* data, net::NetLog* net_log);
  virtual ~MockUDPClientSocket();

  
  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   const CompletionCallback& callback) OVERRIDE;
  virtual int Write(IOBuffer* buf,
                    int buf_len,
                    const CompletionCallback& callback) OVERRIDE;
  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;

  
  virtual void Close() OVERRIDE;
  virtual int GetPeerAddress(IPEndPoint* address) const OVERRIDE;
  virtual int GetLocalAddress(IPEndPoint* address) const OVERRIDE;
  virtual const BoundNetLog& NetLog() const OVERRIDE;

  
  virtual int Connect(const IPEndPoint& address) OVERRIDE;

  
  virtual void OnReadComplete(const MockRead& data) OVERRIDE;
  virtual void OnConnectComplete(const MockConnect& data) OVERRIDE;

 private:
  int CompleteRead();

  void RunCallbackAsync(const CompletionCallback& callback, int result);
  void RunCallback(const CompletionCallback& callback, int result);

  bool connected_;
  SocketDataProvider* data_;
  int read_offset_;
  MockRead read_data_;
  bool need_read_data_;

  
  IPEndPoint peer_addr_;

  
  scoped_refptr<IOBuffer> pending_buf_;
  int pending_buf_len_;
  CompletionCallback pending_callback_;

  BoundNetLog net_log_;

  base::WeakPtrFactory<MockUDPClientSocket> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(MockUDPClientSocket);
};

class TestSocketRequest : public TestCompletionCallbackBase {
 public:
  TestSocketRequest(std::vector<TestSocketRequest*>* request_order,
                    size_t* completion_count);
  virtual ~TestSocketRequest();

  ClientSocketHandle* handle() { return &handle_; }

  const net::CompletionCallback& callback() const { return callback_; }

 private:
  void OnComplete(int result);

  ClientSocketHandle handle_;
  std::vector<TestSocketRequest*>* request_order_;
  size_t* completion_count_;
  CompletionCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(TestSocketRequest);
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

  template <typename PoolType>
  int StartRequestUsingPool(
      PoolType* socket_pool,
      const std::string& group_name,
      RequestPriority priority,
      const scoped_refptr<typename PoolType::SocketParams>& socket_params) {
    DCHECK(socket_pool);
    TestSocketRequest* request =
        new TestSocketRequest(&request_order_, &completion_count_);
    requests_.push_back(request);
    int rv = request->handle()->Init(group_name,
                                     socket_params,
                                     priority,
                                     request->callback(),
                                     socket_pool,
                                     BoundNetLog());
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

  DISALLOW_COPY_AND_ASSIGN(ClientSocketPoolTest);
};

class MockTransportSocketParams
    : public base::RefCounted<MockTransportSocketParams> {
 private:
  friend class base::RefCounted<MockTransportSocketParams>;
  ~MockTransportSocketParams() {}

  DISALLOW_COPY_AND_ASSIGN(MockTransportSocketParams);
};

class MockTransportClientSocketPool : public TransportClientSocketPool {
 public:
  typedef MockTransportSocketParams SocketParams;

  class MockConnectJob {
   public:
    MockConnectJob(scoped_ptr<StreamSocket> socket,
                   ClientSocketHandle* handle,
                   const CompletionCallback& callback);
    ~MockConnectJob();

    int Connect();
    bool CancelHandle(const ClientSocketHandle* handle);

   private:
    void OnConnect(int rv);

    scoped_ptr<StreamSocket> socket_;
    ClientSocketHandle* handle_;
    CompletionCallback user_callback_;

    DISALLOW_COPY_AND_ASSIGN(MockConnectJob);
  };

  MockTransportClientSocketPool(int max_sockets,
                                int max_sockets_per_group,
                                ClientSocketPoolHistograms* histograms,
                                ClientSocketFactory* socket_factory);

  virtual ~MockTransportClientSocketPool();

  RequestPriority last_request_priority() const {
    return last_request_priority_;
  }
  int release_count() const { return release_count_; }
  int cancel_count() const { return cancel_count_; }

  
  virtual int RequestSocket(const std::string& group_name,
                            const void* socket_params,
                            RequestPriority priority,
                            ClientSocketHandle* handle,
                            const CompletionCallback& callback,
                            const BoundNetLog& net_log) OVERRIDE;

  virtual void CancelRequest(const std::string& group_name,
                             ClientSocketHandle* handle) OVERRIDE;
  virtual void ReleaseSocket(const std::string& group_name,
                             scoped_ptr<StreamSocket> socket,
                             int id) OVERRIDE;

 private:
  ClientSocketFactory* client_socket_factory_;
  ScopedVector<MockConnectJob> job_list_;
  RequestPriority last_request_priority_;
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
  std::vector<DeterministicMockUDPClientSocket*>& udp_client_sockets() {
    return udp_client_sockets_;
  }

  
  virtual scoped_ptr<DatagramClientSocket> CreateDatagramClientSocket(
      DatagramSocket::BindType bind_type,
      const RandIntCallback& rand_int_cb,
      NetLog* net_log,
      const NetLog::Source& source) OVERRIDE;
  virtual scoped_ptr<StreamSocket> CreateTransportClientSocket(
      const AddressList& addresses,
      NetLog* net_log,
      const NetLog::Source& source) OVERRIDE;
  virtual scoped_ptr<SSLClientSocket> CreateSSLClientSocket(
      scoped_ptr<ClientSocketHandle> transport_socket,
      const HostPortPair& host_and_port,
      const SSLConfig& ssl_config,
      const SSLClientSocketContext& context) OVERRIDE;
  virtual void ClearSSLSessionCache() OVERRIDE;

 private:
  SocketDataProviderArray<DeterministicSocketData> mock_data_;
  SocketDataProviderArray<SSLSocketDataProvider> mock_ssl_data_;

  
  std::vector<DeterministicMockTCPClientSocket*> tcp_client_sockets_;
  std::vector<DeterministicMockUDPClientSocket*> udp_client_sockets_;
  std::vector<MockSSLClientSocket*> ssl_client_sockets_;

  DISALLOW_COPY_AND_ASSIGN(DeterministicMockClientSocketFactory);
};

class MockSOCKSClientSocketPool : public SOCKSClientSocketPool {
 public:
  MockSOCKSClientSocketPool(int max_sockets,
                            int max_sockets_per_group,
                            ClientSocketPoolHistograms* histograms,
                            TransportClientSocketPool* transport_pool);

  virtual ~MockSOCKSClientSocketPool();

  
  virtual int RequestSocket(const std::string& group_name,
                            const void* socket_params,
                            RequestPriority priority,
                            ClientSocketHandle* handle,
                            const CompletionCallback& callback,
                            const BoundNetLog& net_log) OVERRIDE;

  virtual void CancelRequest(const std::string& group_name,
                             ClientSocketHandle* handle) OVERRIDE;
  virtual void ReleaseSocket(const std::string& group_name,
                             scoped_ptr<StreamSocket> socket,
                             int id) OVERRIDE;

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
