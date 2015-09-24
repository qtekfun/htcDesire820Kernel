// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_CLIENT_SOCKET_HANDLE_H_
#define NET_SOCKET_CLIENT_SOCKET_HANDLE_H_
#pragma once

#include <string>

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/net_errors.h"
#include "net/base/net_log.h"
#include "net/base/request_priority.h"
#include "net/http/http_response_info.h"
#include "net/socket/client_socket.h"
#include "net/socket/client_socket_pool.h"

namespace net {

class ClientSocketHandle {
 public:
  enum SocketReuseType {
    UNUSED = 0,   
    UNUSED_IDLE,  
    REUSED_IDLE,  
    NUM_TYPES,
  };

  ClientSocketHandle();
  ~ClientSocketHandle();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template <typename SocketParams, typename PoolType>
  int Init(const std::string& group_name,
           const scoped_refptr<SocketParams>& socket_params,
           RequestPriority priority,
           CompletionCallback* callback,
           PoolType* pool,
           const BoundNetLog& net_log);

  
  
  
  
  
  
  
  
  void Reset();

  
  
  LoadState GetLoadState() const;

  
  bool is_initialized() const { return is_initialized_; }

  
  base::TimeTicks init_time() const { return init_time_; }

  
  base::TimeDelta setup_time() const { return setup_time_; }

  
  void set_is_reused(bool is_reused) { is_reused_ = is_reused; }
  void set_socket(ClientSocket* s) { socket_.reset(s); }
  void set_idle_time(base::TimeDelta idle_time) { idle_time_ = idle_time; }
  void set_pool_id(int id) { pool_id_ = id; }
  void set_is_ssl_error(bool is_ssl_error) { is_ssl_error_ = is_ssl_error; }
  void set_ssl_error_response_info(const HttpResponseInfo& ssl_error_state) {
    ssl_error_response_info_ = ssl_error_state;
  }
  void set_pending_http_proxy_connection(ClientSocketHandle* connection) {
    pending_http_proxy_connection_.reset(connection);
  }

  
  bool is_ssl_error() const {
    DCHECK(socket_.get() == NULL);
    return is_ssl_error_;
  }
  
  
  
  const HttpResponseInfo& ssl_error_response_info() const {
    return ssl_error_response_info_;
  }
  ClientSocketHandle* release_pending_http_proxy_connection() {
    return pending_http_proxy_connection_.release();
  }

  
  const std::string& group_name() const { return group_name_; }
  int id() const { return pool_id_; }
  ClientSocket* socket() { return socket_.get(); }
  ClientSocket* release_socket() { return socket_.release(); }
  bool is_reused() const { return is_reused_; }
  base::TimeDelta idle_time() const { return idle_time_; }
  SocketReuseType reuse_type() const {
    if (is_reused()) {
      return REUSED_IDLE;
    } else if (idle_time() == base::TimeDelta()) {
      return UNUSED;
    } else {
      return UNUSED_IDLE;
    }
  }

 private:
  
  void OnIOComplete(int result);

  
  
  void HandleInitCompletion(int result);

  
  
  
  void ResetInternal(bool cancel);

  
  void ResetErrorState();

  bool is_initialized_;
  ClientSocketPool* pool_;
  scoped_ptr<ClientSocket> socket_;
  std::string group_name_;
  bool is_reused_;
  CompletionCallbackImpl<ClientSocketHandle> callback_;
  CompletionCallback* user_callback_;
  base::TimeDelta idle_time_;
  int pool_id_;  
  bool is_ssl_error_;
  HttpResponseInfo ssl_error_response_info_;
  scoped_ptr<ClientSocketHandle> pending_http_proxy_connection_;
  base::TimeTicks init_time_;
  base::TimeDelta setup_time_;

  NetLog::Source requesting_source_;

  DISALLOW_COPY_AND_ASSIGN(ClientSocketHandle);
};

template <typename SocketParams, typename PoolType>
int ClientSocketHandle::Init(const std::string& group_name,
                             const scoped_refptr<SocketParams>& socket_params,
                             RequestPriority priority,
                             CompletionCallback* callback,
                             PoolType* pool,
                             const BoundNetLog& net_log) {
  requesting_source_ = net_log.source();

  CHECK(!group_name.empty());
  
  
  
  CheckIsValidSocketParamsForPool<PoolType, SocketParams>();
  ResetInternal(true);
  ResetErrorState();
  pool_ = pool;
  group_name_ = group_name;
  init_time_ = base::TimeTicks::Now();
  int rv = pool_->RequestSocket(
      group_name, &socket_params, priority, this, &callback_, net_log);
  if (rv == ERR_IO_PENDING) {
    user_callback_ = callback;
  } else {
    HandleInitCompletion(rv);
  }
  return rv;
}

}  

#endif  
