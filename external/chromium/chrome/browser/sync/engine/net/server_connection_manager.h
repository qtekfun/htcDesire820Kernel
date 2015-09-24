// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_NET_SERVER_CONNECTION_MANAGER_H_
#define CHROME_BROWSER_SYNC_ENGINE_NET_SERVER_CONNECTION_MANAGER_H_
#pragma once

#include <iosfwd>
#include <string>

#include "base/atomicops.h"
#include "base/observer_list_threadsafe.h"
#include "base/string_util.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/sync/syncable/syncable_id.h"
#include "chrome/common/deprecated/event_sys.h"
#include "chrome/common/deprecated/event_sys-inl.h"
#include "chrome/common/net/http_return.h"

namespace syncable {
class WriteTransaction;
class DirectoryManager;
}

namespace sync_pb {
class ClientToServerMessage;
}

struct RequestTimingInfo;

namespace browser_sync {

class ClientToServerMessage;

static const int32 kMaxConnectionErrorsBeforeReset = 10;

static const int32 kUnsetResponseCode = -1;
static const int32 kUnsetContentLength = -1;
static const int32 kUnsetPayloadLength = -1;

struct HttpResponse {
  enum ServerConnectionCode {
    
    NONE,

    
    CONNECTION_UNAVAILABLE,

    
    IO_ERROR,

    
    
    SYNC_SERVER_ERROR,

    
    
    
    
    
    
    SYNC_AUTH_ERROR,

    
    
    

    
    SERVER_CONNECTION_OK,

    
    
    
    
    
    RETRY,
  };

  
  int64 response_code;

  
  int64 content_length;

  
  int64 payload_length;

  
  std::string update_client_auth_header;

  
  ServerConnectionCode server_status;

  HttpResponse()
      : response_code(kUnsetResponseCode),
        content_length(kUnsetContentLength),
        payload_length(kUnsetPayloadLength),
        server_status(NONE) {}
};

inline bool IsGoodReplyFromServer(HttpResponse::ServerConnectionCode code) {
  return code >= HttpResponse::SERVER_CONNECTION_OK;
}

struct ServerConnectionEvent {
  
  typedef ServerConnectionEvent EventType;
  enum WhatHappened {
    SHUTDOWN,
    STATUS_CHANGED
  };

  static inline bool IsChannelShutdownEvent(const EventType& event) {
    return SHUTDOWN == event.what_happened;
  }

  WhatHappened what_happened;
  HttpResponse::ServerConnectionCode connection_code;
  bool server_reachable;
};

struct ServerConnectionEvent2 {
  HttpResponse::ServerConnectionCode connection_code;
  bool server_reachable;
  ServerConnectionEvent2(HttpResponse::ServerConnectionCode code,
                         bool server_reachable) :
      connection_code(code), server_reachable(server_reachable) {}
};

class ServerConnectionEventListener {
 public:
  virtual void OnServerConnectionEvent(const ServerConnectionEvent2& event) = 0;
 protected:
  virtual ~ServerConnectionEventListener() {}
};

class ServerConnectionManager;
class ScopedServerStatusWatcher {
 public:
  ScopedServerStatusWatcher(ServerConnectionManager* conn_mgr,
                            HttpResponse* response);
  ~ScopedServerStatusWatcher();
 private:
  ServerConnectionManager* const conn_mgr_;
  HttpResponse* const response_;
  
  base::subtle::AtomicWord reset_count_;
  bool server_reachable_;
  DISALLOW_COPY_AND_ASSIGN(ScopedServerStatusWatcher);
};

class ServerConnectionManager {
 public:
  typedef EventChannel<ServerConnectionEvent, base::Lock> Channel;

  
  // buffer_out - string will be overwritten with response
  struct PostBufferParams {
    const std::string& buffer_in;
    std::string* buffer_out;
    HttpResponse* response;
    RequestTimingInfo* timing_info;
  };

  
  
  
  class Post {
   public:
    explicit Post(ServerConnectionManager* scm) : scm_(scm), timing_info_(0) {
    }
    virtual ~Post() { }

    
    virtual bool Init(const char* path,
                      const std::string& auth_token,
                      const std::string& payload,
                      HttpResponse* response) = 0;

    bool ReadBufferResponse(std::string* buffer_out, HttpResponse* response,
                            bool require_response);
    bool ReadDownloadResponse(HttpResponse* response, std::string* buffer_out);

    void set_timing_info(RequestTimingInfo* timing_info) {
      timing_info_ = timing_info;
    }
    RequestTimingInfo* timing_info() { return timing_info_; }

   protected:
    std::string MakeConnectionURL(const std::string& sync_server,
                                  const std::string& path,
                                  bool use_ssl) const;

    void GetServerParams(std::string* server,
                         int* server_port,
                         bool* use_ssl) const {
      base::AutoLock lock(scm_->server_parameters_mutex_);
      server->assign(scm_->sync_server_);
      *server_port = scm_->sync_server_port_;
      *use_ssl = scm_->use_ssl_;
    }

    std::string buffer_;
    ServerConnectionManager* scm_;

   private:
    int ReadResponse(void* buffer, int length);
    int ReadResponse(std::string* buffer, int length);
    RequestTimingInfo* timing_info_;
  };

  ServerConnectionManager(const std::string& server,
                          int port,
                          bool use_ssl,
                          const std::string& user_agent);

  virtual ~ServerConnectionManager();

  
  
  
  
  virtual bool PostBufferWithCachedAuth(const PostBufferParams* params,
                                        ScopedServerStatusWatcher* watcher);

  
  
  virtual bool CheckTime(int32* out_time);

  
  
  virtual bool IsServerReachable();

  
  virtual bool IsUserAuthenticated();

  
  bool CheckServerReachable();

  
  virtual void kill();

  inline Channel* channel() const { return channel_; }

  void AddListener(ServerConnectionEventListener* listener);
  void RemoveListener(ServerConnectionEventListener* listener);

  inline std::string user_agent() const { return user_agent_; }

  inline HttpResponse::ServerConnectionCode server_status() const {
    return server_status_;
  }

  inline bool server_reachable() const { return server_reachable_; }

  const std::string client_id() const { return client_id_; }

  
  
  
  
  
  void SetServerParameters(const std::string& server_url,
                           int port,
                           bool use_ssl);

  
  void GetServerParameters(std::string* server_url,
                           int* port,
                           bool* use_ssl) const;

  std::string GetServerHost() const;

  bool terminate_all_io() const {
    base::AutoLock lock(terminate_all_io_mutex_);
    return terminate_all_io_;
  }

  
  
  virtual Post* MakePost();

  void set_client_id(const std::string& client_id) {
    DCHECK(client_id_.empty());
    client_id_.assign(client_id);
  }

  void set_auth_token(const std::string& auth_token) {
    
    base::AutoLock lock(auth_token_mutex_);
    auth_token_.assign(auth_token);
  }

  const std::string auth_token() const {
    base::AutoLock lock(auth_token_mutex_);
    return auth_token_;
  }

 protected:
  inline std::string proto_sync_path() const {
    base::AutoLock lock(path_mutex_);
    return proto_sync_path_;
  }

  std::string get_time_path() const {
    base::AutoLock lock(path_mutex_);
    return get_time_path_;
  }

  
  
  virtual bool IncrementErrorCount();

  
  
  
  virtual bool PostBufferToPath(const PostBufferParams*,
                                const std::string& path,
                                const std::string& auth_token,
                                ScopedServerStatusWatcher* watcher);

  
  mutable base::Lock server_parameters_mutex_;

  
  std::string sync_server_;

  
  int sync_server_port_;

  
  std::string client_id_;

  
  std::string user_agent_;

  
  bool use_ssl_;

  
  mutable base::Lock path_mutex_;
  std::string proto_sync_path_;
  std::string get_time_path_;

  mutable base::Lock auth_token_mutex_;
  
  std::string auth_token_;

  base::Lock error_count_mutex_;  
  int error_count_;  

  
  Channel* const channel_;

  scoped_refptr<ObserverListThreadSafe<ServerConnectionEventListener> >
     listeners_;

  
  
  volatile HttpResponse::ServerConnectionCode server_status_;
  bool server_reachable_;

  
  volatile base::subtle::AtomicWord reset_count_;

 private:
  friend class Post;
  friend class ScopedServerStatusWatcher;

  void NotifyStatusChanged();
  void ResetConnection();

  mutable base::Lock terminate_all_io_mutex_;
  bool terminate_all_io_;  
  DISALLOW_COPY_AND_ASSIGN(ServerConnectionManager);
};

bool FillMessageWithShareDetails(sync_pb::ClientToServerMessage* csm,
                                 syncable::DirectoryManager* manager,
                                 const std::string& share);

std::ostream& operator<<(std::ostream& s, const struct HttpResponse& hr);

}  

#endif  
