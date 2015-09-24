// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Copyright (c) 2011, Code Aurora Forum. All rights reserved

// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_HTTP_HTTP_CACHE_H_
#define NET_HTTP_HTTP_CACHE_H_
#pragma once

#include <list>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop_proxy.h"
#include "base/task.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/cache_type.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/net_export.h"
#include "net/http/http_transaction_factory.h"

class GURL;

namespace disk_cache {
class Backend;
class Entry;
}

namespace net {

class CertVerifier;
class DnsCertProvenanceChecker;
class DnsRRResolver;
class HostResolver;
class HttpAuthHandlerFactory;
class HttpNetworkSession;
struct HttpRequestInfo;
class HttpResponseInfo;
class IOBuffer;
class NetLog;
class NetworkDelegate;
class ProxyService;
class SSLConfigService;
class ViewCacheHelper;

class NET_EXPORT HttpCache : public HttpTransactionFactory,
                  public base::SupportsWeakPtr<HttpCache>,
                  public base::NonThreadSafe {
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
                              disk_cache::Backend** backend,
                              CompletionCallback* callback,
                              FilePath** stat_db_path) = 0;
  };

  
  class NET_EXPORT DefaultBackend : public BackendFactory {
   public:
    
    
    
    DefaultBackend(CacheType type, const FilePath& path, int max_bytes,
                   base::MessageLoopProxy* thread);
    virtual ~DefaultBackend();

    
    static BackendFactory* InMemory(int max_bytes);

    
    virtual int CreateBackend(NetLog* net_log,
                              disk_cache::Backend** backend,
                              CompletionCallback* callback,
                              FilePath** stat_db_path);

   private:
    CacheType type_;
    const FilePath path_;
    int max_bytes_;
    scoped_refptr<base::MessageLoopProxy> thread_;
  };

  
  
  HttpCache(HostResolver* host_resolver,
            CertVerifier* cert_verifier,
            DnsRRResolver* dnsrr_resolver,
            DnsCertProvenanceChecker* dns_cert_checker,
            ProxyService* proxy_service,
            SSLConfigService* ssl_config_service,
            HttpAuthHandlerFactory* http_auth_handler_factory,
            NetworkDelegate* network_delegate,
            NetLog* net_log,
            BackendFactory* backend_factory);

  
  
  
  
  
  HttpCache(HttpNetworkSession* session, BackendFactory* backend_factory);

  
  
  
  
  HttpCache(HttpTransactionFactory* network_layer,
            NetLog* net_log,
            BackendFactory* backend_factory);

  ~HttpCache();

  HttpTransactionFactory* network_layer() { return network_layer_.get(); }

  
  
  
  
  
  int GetBackend(disk_cache::Backend** backend, CompletionCallback* callback);

  
  disk_cache::Backend* GetCurrentBackend();

  
  static bool ParseResponseInfo(const char* data, int len,
                                HttpResponseInfo* response_info,
                                bool* response_truncated);

  
  
  
  
  void WriteMetadata(const GURL& url, base::Time expected_response_time,
                     IOBuffer* buf, int buf_len);

  
  void set_mode(Mode value) { mode_ = value; }
  Mode mode() { return mode_; }

  
  
  
  void CloseAllConnections();

  
  void CloseIdleConnections();

  
  virtual int CreateTransaction(scoped_ptr<HttpTransaction>* trans);
  virtual HttpCache* GetCache();
  virtual HttpNetworkSession* GetSession();
  virtual void Suspend(bool suspend);

 protected:
  
  enum {
    kResponseInfoIndex = 0,
    kResponseContentIndex,
    kMetadataIndex,

    
    kNumCacheEntryDataIndices
  };
  friend class ViewCacheHelper;

 private:
  

  class BackendCallback;
  class MetadataWriter;
  class SSLHostInfoFactoryAdaptor;
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
                    CompletionCallback* callback);

  
  
  
  
  
  int GetBackendForTransaction(Transaction* trans);

  
  std::string GenerateCacheKey(const HttpRequestInfo*);

  
  
  
  int DoomEntry(const std::string& key, Transaction* trans);

  
  
  
  int AsyncDoomEntry(const std::string& key, Transaction* trans);

  
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

  
  void OnBackendCreated(int result, PendingOp* pending_op);


  

  NetLog* net_log_;

  
  scoped_ptr<BackendFactory> backend_factory_;
  bool building_backend_;

  Mode mode_;

  const scoped_ptr<SSLHostInfoFactoryAdaptor> ssl_host_info_factory_;

  const scoped_ptr<HttpTransactionFactory> network_layer_;
  scoped_ptr<disk_cache::Backend> disk_cache_;

  
  ActiveEntriesMap active_entries_;

  
  ActiveEntriesSet doomed_entries_;

  
  PendingOpsMap pending_ops_;

  ScopedRunnableMethodFactory<HttpCache> task_factory_;

  scoped_ptr<PlaybackCacheMap> playback_cache_map_;

  FilePath* stat_db_path_;

  DISALLOW_COPY_AND_ASSIGN(HttpCache);
};

}  

#endif  
