// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_CLIENT_SOCKET_POOL_H_
#define NET_SOCKET_CLIENT_SOCKET_POOL_H_
#pragma once

#include <deque>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/time.h"
#include "base/template_util.h"
#include "net/base/completion_callback.h"
#include "net/base/host_resolver.h"
#include "net/base/load_states.h"
#include "net/base/request_priority.h"

class DictionaryValue;

namespace net {

class ClientSocket;
class ClientSocketHandle;
class ClientSocketPoolHistograms;

class ClientSocketPool {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int RequestSocket(const std::string& group_name,
                            const void* params,
                            RequestPriority priority,
                            ClientSocketHandle* handle,
                            CompletionCallback* callback,
                            const BoundNetLog& net_log) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void RequestSockets(const std::string& group_name,
                              const void* params,
                              int num_sockets,
                              const BoundNetLog& net_log) = 0;

  
  
  
  
  
  virtual void CancelRequest(const std::string& group_name,
                             ClientSocketHandle* handle) = 0;

  
  
  
  
  
  
  
  virtual void ReleaseSocket(const std::string& group_name,
                             ClientSocket* socket,
                             int id) = 0;

  
  
  
  
  virtual void Flush() = 0;

  
  virtual void CloseIdleSockets() = 0;

  
  virtual int IdleSocketCount() const = 0;

  
  virtual int IdleSocketCountInGroup(const std::string& group_name) const = 0;

  
  virtual LoadState GetLoadState(const std::string& group_name,
                                 const ClientSocketHandle* handle) const = 0;

  
  
  
  
  virtual DictionaryValue* GetInfoAsValue(const std::string& name,
                                          const std::string& type,
                                          bool include_nested_pools) const = 0;

  
  static const int kMaxConnectRetryIntervalMs = 250;

  
  
  virtual ClientSocketPoolHistograms* histograms() const = 0;

  static int unused_idle_socket_timeout();
  static void set_unused_idle_socket_timeout(int timeout);

 protected:
  ClientSocketPool();
  virtual ~ClientSocketPool();

  
  virtual base::TimeDelta ConnectionTimeout() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ClientSocketPool);
};

template <typename PoolType, typename SocketParams>
struct SocketParamTraits : public base::false_type {
};

template <typename PoolType, typename SocketParams>
void CheckIsValidSocketParamsForPool() {
  COMPILE_ASSERT(!base::is_pointer<scoped_refptr<SocketParams> >::value,
                 socket_params_cannot_be_pointer);
  COMPILE_ASSERT((SocketParamTraits<PoolType,
                                    scoped_refptr<SocketParams> >::value),
                 invalid_socket_params_for_pool);
}

#define REGISTER_SOCKET_PARAMS_FOR_POOL(pool_type, socket_params)             \
template<>                                                                    \
struct SocketParamTraits<pool_type, scoped_refptr<socket_params> >            \
    : public base::true_type {                                                \
}

template <typename PoolType, typename SocketParams>
void RequestSocketsForPool(PoolType* pool,
                           const std::string& group_name,
                           const scoped_refptr<SocketParams>& params,
                           int num_sockets,
                           const BoundNetLog& net_log) {
  CheckIsValidSocketParamsForPool<PoolType, SocketParams>();
  pool->RequestSockets(group_name, &params, num_sockets, net_log);
}

}  

#endif  
