// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_MDNS_CLIENT_IMPL_H_
#define NET_DNS_MDNS_CLIENT_IMPL_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/cancelable_callback.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "net/base/io_buffer.h"
#include "net/base/ip_endpoint.h"
#include "net/dns/mdns_cache.h"
#include "net/dns/mdns_client.h"
#include "net/udp/datagram_server_socket.h"
#include "net/udp/udp_server_socket.h"
#include "net/udp/udp_socket.h"

namespace net {

class MDnsSocketFactoryImpl : public MDnsSocketFactory {
 public:
  MDnsSocketFactoryImpl() {};
  virtual ~MDnsSocketFactoryImpl() {};

  virtual void CreateSockets(
      ScopedVector<DatagramServerSocket>* sockets) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(MDnsSocketFactoryImpl);
};

class NET_EXPORT_PRIVATE MDnsConnection {
 public:
  class Delegate {
   public:
    
    virtual void HandlePacket(DnsResponse* response, int bytes_read) = 0;
    virtual void OnConnectionError(int error) = 0;
    virtual ~Delegate() {}
  };

  explicit MDnsConnection(MDnsConnection::Delegate* delegate);
  virtual ~MDnsConnection();

  
  bool Init(MDnsSocketFactory* socket_factory);
  bool Send(IOBuffer* buffer, unsigned size);

 private:
  class SocketHandler {
   public:
    SocketHandler(scoped_ptr<DatagramServerSocket> socket,
                  MDnsConnection* connection);
    ~SocketHandler();

    int Start();
    int Send(IOBuffer* buffer, unsigned size);

   private:
    int DoLoop(int rv);
    void OnDatagramReceived(int rv);

    
    void SendDone(int rv);

    scoped_ptr<DatagramServerSocket> socket_;
    MDnsConnection* connection_;
    IPEndPoint recv_addr_;
    DnsResponse response_;
    IPEndPoint multicast_addr_;

    DISALLOW_COPY_AND_ASSIGN(SocketHandler);
  };

  
  void OnDatagramReceived(DnsResponse* response,
                          const IPEndPoint& recv_addr,
                          int bytes_read);

  void OnError(SocketHandler* loop, int error);

  
  ScopedVector<SocketHandler> socket_handlers_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(MDnsConnection);
};

class MDnsListenerImpl;

class NET_EXPORT_PRIVATE MDnsClientImpl : public MDnsClient {
 public:
  
  
  
  
  class Core : public base::SupportsWeakPtr<Core>, MDnsConnection::Delegate {
   public:
    explicit Core(MDnsClientImpl* client);
    virtual ~Core();

    
    bool Init(MDnsSocketFactory* socket_factory);

    
    bool SendQuery(uint16 rrtype, std::string name);

    
    void AddListener(MDnsListenerImpl* listener);
    void RemoveListener(MDnsListenerImpl* listener);

    
    void QueryCache(uint16 rrtype, const std::string& name,
                    std::vector<const RecordParsed*>* records) const;

    
    virtual void HandlePacket(DnsResponse* response, int bytes_read) OVERRIDE;

    virtual void OnConnectionError(int error) OVERRIDE;

   private:
    typedef std::pair<std::string, uint16> ListenerKey;
    typedef std::map<ListenerKey, ObserverList<MDnsListenerImpl>* >
    ListenerMap;

    
    void AlertListeners(MDnsListener::UpdateType update_type,
                        const ListenerKey& key, const RecordParsed* record);

    
    void ScheduleCleanup(base::Time cleanup);

    
    void DoCleanup();

    
    void OnRecordRemoved(const RecordParsed* record);

    void NotifyNsecRecord(const RecordParsed* record);

    
    
    void CleanupObserverList(const ListenerKey& key);

    ListenerMap listeners_;

    MDnsClientImpl* client_;
    MDnsCache cache_;

    base::CancelableCallback<void()> cleanup_callback_;
    base::Time scheduled_cleanup_;

    scoped_ptr<MDnsConnection> connection_;

    DISALLOW_COPY_AND_ASSIGN(Core);
  };

  MDnsClientImpl();
  virtual ~MDnsClientImpl();

  
  virtual scoped_ptr<MDnsListener> CreateListener(
      uint16 rrtype,
      const std::string& name,
      MDnsListener::Delegate* delegate) OVERRIDE;

  virtual scoped_ptr<MDnsTransaction> CreateTransaction(
      uint16 rrtype,
      const std::string& name,
      int flags,
      const MDnsTransaction::ResultCallback& callback) OVERRIDE;

  virtual bool StartListening(MDnsSocketFactory* socket_factory) OVERRIDE;
  virtual void StopListening() OVERRIDE;
  virtual bool IsListening() const OVERRIDE;

  Core* core() { return core_.get(); }

 private:
  scoped_ptr<Core> core_;

  DISALLOW_COPY_AND_ASSIGN(MDnsClientImpl);
};

class MDnsListenerImpl : public MDnsListener,
                         public base::SupportsWeakPtr<MDnsListenerImpl> {
 public:
  MDnsListenerImpl(uint16 rrtype,
                   const std::string& name,
                   MDnsListener::Delegate* delegate,
                   MDnsClientImpl* client);

  virtual ~MDnsListenerImpl();

  
  virtual bool Start() OVERRIDE;

  virtual const std::string& GetName() const OVERRIDE;

  virtual uint16 GetType() const OVERRIDE;

  MDnsListener::Delegate* delegate() { return delegate_; }

  
  void AlertDelegate(MDnsListener::UpdateType update_type,
                     const RecordParsed* record_parsed);

  
  void AlertNsecRecord();

 private:
  uint16 rrtype_;
  std::string name_;
  MDnsClientImpl* client_;
  MDnsListener::Delegate* delegate_;

  bool started_;
  DISALLOW_COPY_AND_ASSIGN(MDnsListenerImpl);
};

class MDnsTransactionImpl : public base::SupportsWeakPtr<MDnsTransactionImpl>,
                            public MDnsTransaction,
                            public MDnsListener::Delegate {
 public:
  MDnsTransactionImpl(uint16 rrtype,
                      const std::string& name,
                      int flags,
                      const MDnsTransaction::ResultCallback& callback,
                      MDnsClientImpl* client);
  virtual ~MDnsTransactionImpl();

  
  virtual bool Start() OVERRIDE;

  virtual const std::string& GetName() const OVERRIDE;
  virtual uint16 GetType() const OVERRIDE;

  
  virtual void OnRecordUpdate(MDnsListener::UpdateType update,
                              const RecordParsed* record) OVERRIDE;
  virtual void OnNsecRecord(const std::string& name, unsigned type) OVERRIDE;

  virtual void OnCachePurged() OVERRIDE;

 private:
  bool is_active() { return !callback_.is_null(); }

  void Reset();

  
  void TriggerCallback(MDnsTransaction::Result result,
                       const RecordParsed* record);

  
  void CacheRecordFound(const RecordParsed* record);

  
  void SignalTransactionOver();

  
  
  void ServeRecordsFromCache();

  
  
  
  bool QueryAndListen();

  uint16 rrtype_;
  std::string name_;
  MDnsTransaction::ResultCallback callback_;

  scoped_ptr<MDnsListener> listener_;
  base::CancelableCallback<void()> timeout_;

  MDnsClientImpl* client_;

  bool started_;
  int flags_;

  DISALLOW_COPY_AND_ASSIGN(MDnsTransactionImpl);
};

}  
#endif  
