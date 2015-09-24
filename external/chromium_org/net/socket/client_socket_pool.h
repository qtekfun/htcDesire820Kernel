// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_CLIENT_SOCKET_POOL_H_
#define NET_SOCKET_CLIENT_SOCKET_POOL_H_

#include <deque>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/template_util.h"
#include "base/time/time.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"
#include "net/dns/host_resolver.h"

namespace base {
class DictionaryValue;
}

namespace net {

class ClientSocketHandle;
class ClientSocketPoolHistograms;
class StreamSocket;

class NET_EXPORT HigherLayeredPool {
 public:
  virtual ~HigherLayeredPool() {}

  
  
  
  virtual bool CloseOneIdleConnection() = 0;
};

class NET_EXPORT LowerLayeredPool {
 public:
  virtual ~LowerLayeredPool() {}

  
  
  
  virtual bool IsStalled() const = 0;

  
  
  
  virtual void AddHigherLayeredPool(HigherLayeredPool* higher_pool) = 0;
  virtual void RemoveHigherLayeredPool(HigherLayeredPool* higher_pool) = 0;
};

class NET_EXPORT ClientSocketPool : public LowerLayeredPool {
 public:
  
  
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int RequestSocket(const std::string& group_name,
                            const void* params,
                            RequestPriority priority,
                            ClientSocketHandle* handle,
                            const CompletionCallback& callback,
                            const BoundNetLog& net_log) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void RequestSockets(const std::string& group_name,
                              const void* params,
                              int num_sockets,
                              const BoundNetLog& net_log) = 0;

  
  
  
  
  
  virtual void CancelRequest(const std::string& group_name,
                             ClientSocketHandle* handle) = 0;

  
  
  
  
  
  
  
  virtual void ReleaseSocket(const std::string& group_name,
                             scoped_ptr<StreamSocket> socket,
                             int id) = 0;

  
  
  
  
  
  virtual void FlushWithError(int error) = 0;

  
  virtual void CloseIdleSockets() = 0;

  
  virtual int IdleSocketCount() const = 0;

  
  virtual int IdleSocketCountInGroup(const std::string& group_name) const = 0;

  
  virtual LoadState GetLoadState(const std::string& group_name,
                                 const ClientSocketHandle* handle) const = 0;

  
  
  
  
  virtual base::DictionaryValue* GetInfoAsValue(
      const std::string& name,
      const std::string& type,
      bool include_nested_pools) const = 0;

  
  static const int kMaxConnectRetryIntervalMs = 250;

  
  
  virtual ClientSocketPoolHistograms* histograms() const = 0;

  static base::TimeDelta unused_idle_socket_timeout();
  static void set_unused_idle_socket_timeout(base::TimeDelta timeout);

  static base::TimeDelta used_idle_socket_timeout();
  static void set_used_idle_socket_timeout(base::TimeDelta timeout);

 protected:
  ClientSocketPool();
  virtual ~ClientSocketPool();

  
  virtual base::TimeDelta ConnectionTimeout() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ClientSocketPool);
};

template <typename PoolType>
void RequestSocketsForPool(
    PoolType* pool,
    const std::string& group_name,
    const scoped_refptr<typename PoolType::SocketParams>& params,
    int num_sockets,
    const BoundNetLog& net_log) {
  pool->RequestSockets(group_name, &params, num_sockets, net_log);
}

}  

#endif  
