// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_HTTP_HTTP_CACHE_H_
#define NET_HTTP_HTTP_CACHE_H_

#include <list>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "net/base/cache_type.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"
#include "net/http/http_network_session.h"
#include "net/http/http_transaction_factory.h"

class GURL;

namespace disk_cache {
class Backend;
class Entry;
}

namespace net {

class CertVerifier;
class HostResolver;
class HttpAuthHandlerFactory;
class HttpNetworkSession;
class HttpResponseInfo;
class HttpServerProperties;
class IOBuffer;
class NetLog;
class NetworkDelegate;
class ServerBoundCertService;
class ProxyService;
class SSLConfigService;
class TransportSecurityState;
class ViewCacheHelper;
struct HttpRequestInfo;

class NET_EXPORT HttpCache : public HttpTransactionFactory,
                             public base::SupportsWeakPtr<HttpCache>,
                             NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  enum Mode {
    
    NORMAL = 0,
    
    RECORD,
    
    
    PLAYBACK,
    
    
    DISABLE
  };

  
  class NET_EXPORT BackendFactory {
   public:
    virtual ~BackendFactory() {}

    
    
    
    
    
    
    virtual int CreateBackend(NetLog* net_log,
                              scoped_ptr<disk_cache::Backend>* backend,
                              const CompletionCallback& callback) = 0;
  };

  
  class NET_EXPORT DefaultBackend : public BackendFactory {
   public:
    
    
    
    DefaultBackend(CacheType type, BackendType backend_type,
                   const base::FilePath& path, int max_bytes,
                   base::MessageLoopProxy* thread);
    virtual ~DefaultBackend();

    
    static BackendFactory* InMemory(int max_bytes);

    
    virtual int CreateBackend(NetLog* net_log,
                              scoped_ptr<disk_cache::Backend>* backend,
                              const CompletionCallback& callback) OVERRIDE;

   private:
    CacheType type_;
    BackendType backend_type_;
    const base::FilePath path_;
    int max_bytes_;
    scoped_refptr<base::MessageLoopProxy> thread_;
  };

  
  
  HttpCache(const net::HttpNetworkSession::Params& params,
            BackendFactory* backend_factory);

  
  
  
  
  
  HttpCache(HttpNetworkSession* session, BackendFactory* backend_factory);

  
  
  
  
  HttpCache(HttpTransactionFactory* network_layer,
            NetLog* net_log,
            BackendFactory* backend_factory);

  virtual ~HttpCache();

  HttpTransactionFactory* network_layer() { return network_layer_.get(); }

  
  
  
  
  
  int GetBackend(disk_cache::Backend** backend,
                 const net::CompletionCallback& callback);

  
  disk_cache::Backend* GetCurrentBackend() const;

  
  static bool ParseResponseInfo(const char* data, int len,
                                HttpResponseInfo* response_info,
                                bool* response_truncated);

  
  
  
  
  void WriteMetadata(const GURL& url,
                     RequestPriority priority,
                     base::Time expected_response_time,
                     IOBuffer* buf,
                     int buf_len);

  
  void set_mode(Mode value) { mode_ = value; }
  Mode mode() { return mode_; }

  
  
  
  void CloseAllConnections();

  
  void CloseIdleConnections();

  
  
  void OnExternalCacheHit(const GURL& url, const std::string& http_method);

  
  void InitializeInfiniteCache(const base::FilePath& path);

  
  virtual int CreateTransaction(RequestPriority priority,
                                scoped_ptr<HttpTransaction>* trans,
                                HttpTransactionDelegate* delegate) OVERRIDE;
  virtual HttpCache* GetCache() OVERRIDE;
  virtual HttpNetworkSession* GetSession() OVERRIDE;

 protected:
  
  enum {
    kResponseInfoIndex = 0,
    kResponseContentIndex,
    kMetadataIndex,

    
    kNumCacheEntryDataIndices
  };
  friend class ViewCacheHelper;

 private:
  

  class MetadataWriter;
  class Transaction;
  class WorkItem;
  friend class Transaction;
  struct PendingOp;  

  typedef std::list<Transaction*> TransactionList;
  typedef std::list<WorkItem*> WorkItemList;

  struct ActiveEntry {
    explicit ActiveEntry(disk_cache::Entry* entry);
    ~ActiveEntry();

    disk_cache::Entry* disk_entry;
    Transaction*       writer;
    TransactionList    readers;
    TransactionList    pending_queue;
    bool               will_process_pending_queue;
    bool               doomed;
  };

  typedef base::hash_map<std::string, ActiveEntry*> ActiveEntriesMap;
  typedef base::hash_map<std::string, PendingOp*> PendingOpsMap;
  typedef std::set<ActiveEntry*> ActiveEntriesSet;
  typedef base::hash_map<std::string, int> PlaybackCacheMap;

  

  
  
  int CreateBackend(disk_cache::Backend** backend,
                    const net::CompletionCallback& callback);

  
  
  
  
  
  int GetBackendForTransaction(Transaction* trans);

  
  std::string GenerateCacheKey(const HttpRequestInfo*);

  
  
  void DoomActiveEntry(const std::string& key);

  
  
  
  int DoomEntry(const std::string& key, Transaction* trans);

  
  
  
  int AsyncDoomEntry(const std::string& key, Transaction* trans);

  
  void DoomMainEntryForUrl(const GURL& url);

  
  void FinalizeDoomedEntry(ActiveEntry* entry);

  
  ActiveEntry* FindActiveEntry(const std::string& key);

  
  
  ActiveEntry* ActivateEntry(disk_cache::Entry* disk_entry);

  
  void DeactivateEntry(ActiveEntry* entry);

  
  void SlowDeactivateEntry(ActiveEntry* entry);

  
  
  PendingOp* GetPendingOp(const std::string& key);

  
  void DeletePendingOp(PendingOp* pending_op);

  
  
  
  int OpenEntry(const std::string& key, ActiveEntry** entry,
                Transaction* trans);

  
  
  
  int CreateEntry(const std::string& key, ActiveEntry** entry,
                  Transaction* trans);

  
  void DestroyEntry(ActiveEntry* entry);

  
  
  
  
  
  int AddTransactionToEntry(ActiveEntry* entry, Transaction* trans);

  
  
  
  void DoneWithEntry(ActiveEntry* entry, Transaction* trans, bool cancel);

  
  
  void DoneWritingToEntry(ActiveEntry* entry, bool success);

  
  void DoneReadingFromEntry(ActiveEntry* entry, Transaction* trans);

  
  
  void ConvertWriterToReader(ActiveEntry* entry);

  
  LoadState GetLoadStateForPendingTransaction(const Transaction* trans);

  
  
  void RemovePendingTransaction(Transaction* trans);

  
  bool RemovePendingTransactionFromEntry(ActiveEntry* entry,
                                         Transaction* trans);

  
  bool RemovePendingTransactionFromPendingOp(PendingOp* pending_op,
                                             Transaction* trans);

  
  void ProcessPendingQueue(ActiveEntry* entry);

  

  void OnProcessPendingQueue(ActiveEntry* entry);

  

  
  void OnIOComplete(int result, PendingOp* entry);

  
  
  
  
  
  
  static void OnPendingOpComplete(const base::WeakPtr<HttpCache>& cache,
                                  PendingOp* pending_op,
                                  int result);

  
  void OnBackendCreated(int result, PendingOp* pending_op);

  

  NetLog* net_log_;

  
  scoped_ptr<BackendFactory> backend_factory_;
  bool building_backend_;

  Mode mode_;

  const scoped_ptr<HttpTransactionFactory> network_layer_;
  scoped_ptr<disk_cache::Backend> disk_cache_;

  
  ActiveEntriesMap active_entries_;

  
  ActiveEntriesSet doomed_entries_;

  
  PendingOpsMap pending_ops_;

  scoped_ptr<PlaybackCacheMap> playback_cache_map_;

  DISALLOW_COPY_AND_ASSIGN(HttpCache);
};

}  

#endif  
