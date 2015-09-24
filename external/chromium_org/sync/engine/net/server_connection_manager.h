// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_NET_SERVER_CONNECTION_MANAGER_H_
#define SYNC_ENGINE_NET_SERVER_CONNECTION_MANAGER_H_

#include <iosfwd>
#include <string>

#include "base/atomicops.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/strings/string_util.h"
#include "base/synchronization/lock.h"
#include "base/threading/non_thread_safe.h"
#include "base/threading/thread_checker.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/cancelation_observer.h"
#include "sync/syncable/syncable_id.h"

namespace sync_pb {
class ClientToServerMessage;
}

namespace syncer {

class CancelationSignal;

namespace syncable {
class Directory;
}

static const int32 kUnsetResponseCode = -1;
static const int32 kUnsetContentLength = -1;
static const int32 kUnsetPayloadLength = -1;

struct SYNC_EXPORT_PRIVATE HttpResponse {
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

  
  ServerConnectionCode server_status;

  HttpResponse();

  static const char* GetServerConnectionCodeString(
      ServerConnectionCode code);

  static ServerConnectionCode ServerConnectionCodeFromNetError(
      int error_code);
};

struct ServerConnectionEvent {
  HttpResponse::ServerConnectionCode connection_code;
  explicit ServerConnectionEvent(HttpResponse::ServerConnectionCode code) :
      connection_code(code) {}
};

class SYNC_EXPORT_PRIVATE ServerConnectionEventListener {
 public:
  virtual void OnServerConnectionEvent(const ServerConnectionEvent& event) = 0;
 protected:
  virtual ~ServerConnectionEventListener() {}
};

class ServerConnectionManager;
class SYNC_EXPORT_PRIVATE ScopedServerStatusWatcher
    : public base::NonThreadSafe {
 public:
  ScopedServerStatusWatcher(ServerConnectionManager* conn_mgr,
                            HttpResponse* response);
  virtual ~ScopedServerStatusWatcher();
 private:
  ServerConnectionManager* const conn_mgr_;
  HttpResponse* const response_;
  DISALLOW_COPY_AND_ASSIGN(ScopedServerStatusWatcher);
};

class SYNC_EXPORT_PRIVATE ServerConnectionManager : public CancelationObserver {
 public:
  
  // buffer_out - string will be overwritten with response
  struct PostBufferParams {
    std::string buffer_in;
    std::string buffer_out;
    HttpResponse response;
  };

  
  
  
  class Connection {
   public:
    explicit Connection(ServerConnectionManager* scm);
    virtual ~Connection();

    
    virtual bool Init(const char* path,
                      const std::string& auth_token,
                      const std::string& payload,
                      HttpResponse* response) = 0;

    
    
    virtual void Abort() = 0;

    bool ReadBufferResponse(std::string* buffer_out, HttpResponse* response,
                            bool require_response);
    bool ReadDownloadResponse(HttpResponse* response, std::string* buffer_out);

   protected:
    std::string MakeConnectionURL(const std::string& sync_server,
                                  const std::string& path,
                                  bool use_ssl) const;

    void GetServerParams(std::string* server,
                         int* server_port,
                         bool* use_ssl) const {
      server->assign(scm_->sync_server_);
      *server_port = scm_->sync_server_port_;
      *use_ssl = scm_->use_ssl_;
    }

    std::string buffer_;
    ServerConnectionManager* scm_;

   private:
    int ReadResponse(void* buffer, int length);
    int ReadResponse(std::string* buffer, int length);
  };

  ServerConnectionManager(const std::string& server,
                          int port,
                          bool use_ssl,
                          CancelationSignal* cancelation_signal);

  virtual ~ServerConnectionManager();

  
  
  
  
  virtual bool PostBufferWithCachedAuth(PostBufferParams* params,
                                        ScopedServerStatusWatcher* watcher);

  void AddListener(ServerConnectionEventListener* listener);
  void RemoveListener(ServerConnectionEventListener* listener);

  inline HttpResponse::ServerConnectionCode server_status() const {
    DCHECK(thread_checker_.CalledOnValidThread());
    return server_status_;
  }

  const std::string client_id() const { return client_id_; }

  
  void GetServerParameters(std::string* server_url,
                           int* port,
                           bool* use_ssl) const;

  std::string GetServerHost() const;

  
  
  virtual Connection* MakeConnection();

  
  
  
  
  virtual void OnSignalReceived() OVERRIDE FINAL;

  void set_client_id(const std::string& client_id) {
    DCHECK(thread_checker_.CalledOnValidThread());
    DCHECK(client_id_.empty());
    client_id_.assign(client_id);
  }

  
  bool SetAuthToken(const std::string& auth_token);

  
  
  
  
  
  void OnInvalidationCredentialsRejected();

  bool HasInvalidAuthToken() {
    return auth_token_.empty();
  }

  const std::string auth_token() const {
    DCHECK(thread_checker_.CalledOnValidThread());
    return auth_token_;
  }

 protected:
  inline std::string proto_sync_path() const {
    return proto_sync_path_;
  }

  
  void SetServerStatus(HttpResponse::ServerConnectionCode server_status);

  
  
  
  virtual bool PostBufferToPath(PostBufferParams*,
                                const std::string& path,
                                const std::string& auth_token,
                                ScopedServerStatusWatcher* watcher);

  
  
  
  void InvalidateAndClearAuthToken();

  
  
  
  Connection* MakeActiveConnection();

  
  
  void OnConnectionDestroyed(Connection* connection);

  
  std::string sync_server_;

  
  int sync_server_port_;

  
  std::string client_id_;

  
  bool use_ssl_;

  
  std::string proto_sync_path_;

  
  std::string auth_token_;

  
  std::string previously_invalidated_token;

  ObserverList<ServerConnectionEventListener> listeners_;

  HttpResponse::ServerConnectionCode server_status_;

  base::ThreadChecker thread_checker_;

  
  base::Lock terminate_connection_lock_;

  
  
  bool terminated_;

  
  
  Connection* active_connection_;

 private:
  friend class Connection;
  friend class ScopedServerStatusWatcher;

  
  
  
  
  class ScopedConnectionHelper {
   public:
    
    ScopedConnectionHelper(ServerConnectionManager* manager,
                           Connection* connection);
    ~ScopedConnectionHelper();
    Connection* get();
   private:
    ServerConnectionManager* manager_;
    scoped_ptr<Connection> connection_;
    DISALLOW_COPY_AND_ASSIGN(ScopedConnectionHelper);
  };

  void NotifyStatusChanged();

  CancelationSignal* const cancelation_signal_;
  bool signal_handler_registered_;

  DISALLOW_COPY_AND_ASSIGN(ServerConnectionManager);
};

std::ostream& operator<<(std::ostream& s, const struct HttpResponse& hr);

}  

#endif  
