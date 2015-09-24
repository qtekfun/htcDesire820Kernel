// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_CLIENT_SOCKET_HANDLE_H_
#define NET_SOCKET_CLIENT_SOCKET_HANDLE_H_

#include <string>

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/load_timing_info.h"
#include "net/base/net_errors.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/request_priority.h"
#include "net/http/http_response_info.h"
#include "net/socket/client_socket_pool.h"
#include "net/socket/stream_socket.h"

namespace net {

class NET_EXPORT ClientSocketHandle {
 public:
  enum SocketReuseType {
    UNUSED = 0,   
    UNUSED_IDLE,  
    REUSED_IDLE,  
    NUM_TYPES,
  };

  ClientSocketHandle();
  ~ClientSocketHandle();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template <typename PoolType>
  int Init(const std::string& group_name,
           const scoped_refptr<typename PoolType::SocketParams>& socket_params,
           RequestPriority priority,
           const CompletionCallback& callback,
           PoolType* pool,
           const BoundNetLog& net_log);

  
  
  
  
  
  
  
  
  void Reset();

  
  
  LoadState GetLoadState() const;

  bool IsPoolStalled() const;

  
  
  
  
  void AddHigherLayeredPool(HigherLayeredPool* higher_pool);

  
  
  void RemoveHigherLayeredPool(HigherLayeredPool* higher_pool);

  
  bool is_initialized() const { return is_initialized_; }

  
  base::TimeTicks init_time() const { return init_time_; }

  
  base::TimeDelta setup_time() const { return setup_time_; }

  
  
  
  
  
  bool GetLoadTimingInfo(bool is_reused,
                         LoadTimingInfo* load_timing_info) const;

  
  
  
  
  void SetSocket(scoped_ptr<StreamSocket> s);
  void set_is_reused(bool is_reused) { is_reused_ = is_reused; }
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

  StreamSocket* socket() { return socket_.get(); }

  
  
  scoped_ptr<StreamSocket> PassSocket();

  
  const std::string& group_name() const { return group_name_; }
  int id() const { return pool_id_; }
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
  const LoadTimingInfo::ConnectTiming& connect_timing() const {
    return connect_timing_;
  }
  void set_connect_timing(const LoadTimingInfo::ConnectTiming& connect_timing) {
    connect_timing_ = connect_timing;
  }

 private:
  
  void OnIOComplete(int result);

  
  
  void HandleInitCompletion(int result);

  
  
  
  void ResetInternal(bool cancel);

  
  void ResetErrorState();

  bool is_initialized_;
  ClientSocketPool* pool_;
  HigherLayeredPool* higher_pool_;
  scoped_ptr<StreamSocket> socket_;
  std::string group_name_;
  bool is_reused_;
  CompletionCallback callback_;
  CompletionCallback user_callback_;
  base::TimeDelta idle_time_;
  int pool_id_;  
  bool is_ssl_error_;
  HttpResponseInfo ssl_error_response_info_;
  scoped_ptr<ClientSocketHandle> pending_http_proxy_connection_;
  base::TimeTicks init_time_;
  base::TimeDelta setup_time_;

  NetLog::Source requesting_source_;

  
  LoadTimingInfo::ConnectTiming connect_timing_;

  DISALLOW_COPY_AND_ASSIGN(ClientSocketHandle);
};

template <typename PoolType>
int ClientSocketHandle::Init(
    const std::string& group_name,
    const scoped_refptr<typename PoolType::SocketParams>& socket_params,
    RequestPriority priority,
    const CompletionCallback& callback,
    PoolType* pool,
    const BoundNetLog& net_log) {
  requesting_source_ = net_log.source();

  CHECK(!group_name.empty());
  ResetInternal(true);
  ResetErrorState();
  pool_ = pool;
  group_name_ = group_name;
  init_time_ = base::TimeTicks::Now();
  int rv = pool_->RequestSocket(
      group_name, &socket_params, priority, this, callback_, net_log);
  if (rv == ERR_IO_PENDING) {
    user_callback_ = callback;
  } else {
    HandleInitCompletion(rv);
  }
  return rv;
}

}  

#endif  
