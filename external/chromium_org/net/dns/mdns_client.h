// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_MDNS_CLIENT_H_
#define NET_DNS_MDNS_CLIENT_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "net/base/ip_endpoint.h"
#include "net/dns/dns_query.h"
#include "net/dns/dns_response.h"
#include "net/dns/record_parsed.h"

namespace net {

class DatagramServerSocket;
class RecordParsed;

class NET_EXPORT MDnsTransaction {
 public:
  
  enum Result {
    
    RESULT_RECORD,
    
    
    
    RESULT_DONE,
    
    
    
    
    
    RESULT_NO_RESULTS,
    
    
    
    RESULT_NSEC
  };

  
  enum Flags {
    
    
    
    SINGLE_RESULT = 1 << 0,
    
    QUERY_CACHE = 1 << 1,
    QUERY_NETWORK = 1 << 2,
    
    
    FLAG_MASK = (1 << 3) - 1,
  };

  typedef base::Callback<void(Result, const RecordParsed*)>
  ResultCallback;

  
  virtual ~MDnsTransaction() {}

  
  
  virtual bool Start() = 0;

  
  virtual const std::string& GetName() const = 0;

  
  virtual uint16 GetType() const = 0;
};

class NET_EXPORT MDnsListener {
 public:
  
  
  enum UpdateType {
    RECORD_ADDED,
    RECORD_CHANGED,
    RECORD_REMOVED
  };

  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnRecordUpdate(UpdateType update,
                                const RecordParsed* record) = 0;

    
    virtual void OnNsecRecord(const std::string& name, unsigned type) = 0;

    
    
    virtual void OnCachePurged() = 0;
  };

  
  virtual ~MDnsListener() {}

  
  virtual bool Start() = 0;

  
  
  virtual const std::string& GetName() const = 0;

  
  virtual uint16 GetType() const = 0;
};

class NET_EXPORT MDnsSocketFactory {
 public:
  virtual ~MDnsSocketFactory() {}
  virtual void CreateSockets(ScopedVector<DatagramServerSocket>* sockets) = 0;

  static scoped_ptr<MDnsSocketFactory> CreateDefault();
};

class NET_EXPORT MDnsClient {
 public:
  virtual ~MDnsClient() {}

  
  virtual scoped_ptr<MDnsListener> CreateListener(
      uint16 rrtype,
      const std::string& name,
      MDnsListener::Delegate* delegate) = 0;

  
  
  
  virtual scoped_ptr<MDnsTransaction> CreateTransaction(
      uint16 rrtype,
      const std::string& name,
      int flags,
      const MDnsTransaction::ResultCallback& callback) = 0;

  virtual bool StartListening(MDnsSocketFactory* factory) = 0;

  
  
  virtual void StopListening() = 0;
  virtual bool IsListening() const = 0;

  
  static scoped_ptr<MDnsClient> CreateDefault();
};

NET_EXPORT IPEndPoint GetMDnsIPEndPoint(AddressFamily address_family);

typedef std::vector<std::pair<uint32, AddressFamily> > InterfaceIndexFamilyList;
NET_EXPORT InterfaceIndexFamilyList GetMDnsInterfacesToBind();

NET_EXPORT scoped_ptr<DatagramServerSocket> CreateAndBindMDnsSocket(
    AddressFamily address_family,
    uint32 interface_index);

}  

#endif  
