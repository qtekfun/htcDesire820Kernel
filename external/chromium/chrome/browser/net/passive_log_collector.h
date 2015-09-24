// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_PASSIVE_LOG_COLLECTOR_H_
#define CHROME_BROWSER_NET_PASSIVE_LOG_COLLECTOR_H_
#pragma once

#include <deque>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/time.h"
#include "chrome/browser/net/chrome_net_log.h"
#include "net/base/net_log.h"

class PassiveLogCollector : public ChromeNetLog::ThreadSafeObserver {
 public:
  typedef std::vector<net::NetLog::Source> SourceDependencyList;

  struct SourceInfo {
    SourceInfo();
    ~SourceInfo();

    
    
    
    std::string GetURL() const;

    uint32 source_id;
    ChromeNetLog::EntryList entries;
    size_t num_entries_truncated;

    
    
    
    SourceDependencyList dependencies;

    
    
    
    int reference_count;

    
    
    
    bool is_alive;
  };

  typedef std::vector<SourceInfo> SourceInfoList;

  
  class SourceTrackerInterface {
   public:
    virtual ~SourceTrackerInterface() {}

    virtual void OnAddEntry(const ChromeNetLog::Entry& entry) = 0;

    
    virtual void Clear() = 0;

    
    virtual void AppendAllEntries(ChromeNetLog::EntryList* out) const = 0;
  };

  
  
  class GlobalSourceTracker : public SourceTrackerInterface {
   public:
    GlobalSourceTracker();
    ~GlobalSourceTracker();

    
    virtual void OnAddEntry(const ChromeNetLog::Entry& entry);
    virtual void Clear();
    virtual void AppendAllEntries(ChromeNetLog::EntryList* out) const;

   private:
    typedef std::deque<ChromeNetLog::Entry> CircularEntryList;
    CircularEntryList entries_;
    DISALLOW_COPY_AND_ASSIGN(GlobalSourceTracker);
  };

  
  
  class SourceTracker : public SourceTrackerInterface {
   public:
    
    
    
    
    
    SourceTracker(size_t max_num_sources,
                  size_t max_graveyard_size,
                  PassiveLogCollector* parent);

    virtual ~SourceTracker();

    
    virtual void OnAddEntry(const ChromeNetLog::Entry& entry);
    virtual void Clear();
    virtual void AppendAllEntries(ChromeNetLog::EntryList* out) const;

#ifdef UNIT_TEST
    
    
    SourceInfoList GetAllDeadOrAliveSources(bool is_alive) const {
      SourceInfoList result;
      for (SourceIDToInfoMap::const_iterator it = sources_.begin();
           it != sources_.end(); ++it) {
        if (it->second.is_alive == is_alive)
          result.push_back(it->second);
      }
      return result;
    }
#endif

   protected:
    enum Action {
      ACTION_NONE,
      ACTION_DELETE,
      ACTION_MOVE_TO_GRAVEYARD,
    };

    
    
    void AddReferenceToSourceDependency(const net::NetLog::Source& source,
                                        SourceInfo* info);

   private:
    typedef base::hash_map<uint32, SourceInfo> SourceIDToInfoMap;
    typedef std::deque<uint32> DeletionQueue;

    
    
    virtual Action DoAddEntry(const ChromeNetLog::Entry& entry,
                              SourceInfo* out_info) = 0;

    
    
    void DeleteSourceInfo(uint32 source_id);

    
    void AddToDeletionQueue(uint32 source_id);

    
    void EraseFromDeletionQueue(uint32 source_id);

    
    
    void AdjustReferenceCountForSource(int offset, uint32 source_id);

    
    void ReleaseAllReferencesToDependencies(SourceInfo* info);

    
    
    SourceIDToInfoMap sources_;

    size_t max_num_sources_;
    size_t max_graveyard_size_;

    
    
    
    DeletionQueue deletion_queue_;

    PassiveLogCollector* parent_;

    DISALLOW_COPY_AND_ASSIGN(SourceTracker);
  };

  
  class ConnectJobTracker : public SourceTracker {
   public:
    static const size_t kMaxNumSources;
    static const size_t kMaxGraveyardSize;

    explicit ConnectJobTracker(PassiveLogCollector* parent);

   private:
    virtual Action DoAddEntry(const ChromeNetLog::Entry& entry,
                              SourceInfo* out_info);
    DISALLOW_COPY_AND_ASSIGN(ConnectJobTracker);
  };

  
  class SocketTracker : public SourceTracker {
   public:
    static const size_t kMaxNumSources;
    static const size_t kMaxGraveyardSize;

    SocketTracker();

   private:
    virtual Action DoAddEntry(const ChromeNetLog::Entry& entry,
                              SourceInfo* out_info);

    DISALLOW_COPY_AND_ASSIGN(SocketTracker);
  };

  
  class RequestTracker : public SourceTracker {
   public:
    static const size_t kMaxNumSources;
    static const size_t kMaxGraveyardSize;

    explicit RequestTracker(PassiveLogCollector* parent);

   private:
    virtual Action DoAddEntry(const ChromeNetLog::Entry& entry,
                              SourceInfo* out_info);

    DISALLOW_COPY_AND_ASSIGN(RequestTracker);
  };

  
  
  class InitProxyResolverTracker : public SourceTracker {
   public:
    static const size_t kMaxNumSources;
    static const size_t kMaxGraveyardSize;

    InitProxyResolverTracker();

   private:
    virtual Action DoAddEntry(const ChromeNetLog::Entry& entry,
                              SourceInfo* out_info);

    DISALLOW_COPY_AND_ASSIGN(InitProxyResolverTracker);
  };

  
  class SpdySessionTracker : public SourceTracker {
   public:
    static const size_t kMaxNumSources;
    static const size_t kMaxGraveyardSize;

    SpdySessionTracker();

   private:
    virtual Action DoAddEntry(const ChromeNetLog::Entry& entry,
                              SourceInfo* out_info);

    DISALLOW_COPY_AND_ASSIGN(SpdySessionTracker);
  };

  
  class DNSRequestTracker : public SourceTracker {
   public:
    static const size_t kMaxNumSources;
    static const size_t kMaxGraveyardSize;

    DNSRequestTracker();

   private:
    virtual Action DoAddEntry(const ChromeNetLog::Entry& entry,
                              SourceInfo* out_info);

    DISALLOW_COPY_AND_ASSIGN(DNSRequestTracker);
  };

  
  class DNSJobTracker : public SourceTracker {
   public:
    static const size_t kMaxNumSources;
    static const size_t kMaxGraveyardSize;

    DNSJobTracker();

   private:
    virtual Action DoAddEntry(const ChromeNetLog::Entry& entry,
                              SourceInfo* out_info);

    DISALLOW_COPY_AND_ASSIGN(DNSJobTracker);
  };

  
  class DiskCacheEntryTracker : public SourceTracker {
   public:
    static const size_t kMaxNumSources;
    static const size_t kMaxGraveyardSize;

    DiskCacheEntryTracker();

   private:
    virtual Action DoAddEntry(const ChromeNetLog::Entry& entry,
                              SourceInfo* out_info);

    DISALLOW_COPY_AND_ASSIGN(DiskCacheEntryTracker);
  };

  
  class MemCacheEntryTracker : public SourceTracker {
   public:
    static const size_t kMaxNumSources;
    static const size_t kMaxGraveyardSize;

    MemCacheEntryTracker();

   protected:
    virtual Action DoAddEntry(const ChromeNetLog::Entry& entry,
                              SourceInfo* out_info);

   private:
    DISALLOW_COPY_AND_ASSIGN(MemCacheEntryTracker);
  };

  class HttpStreamJobTracker : public SourceTracker {
   public:
    static const size_t kMaxNumSources;
    static const size_t kMaxGraveyardSize;

    explicit HttpStreamJobTracker(PassiveLogCollector* parent);

   private:
    virtual Action DoAddEntry(const ChromeNetLog::Entry& entry,
                              SourceInfo* out_info);
    DISALLOW_COPY_AND_ASSIGN(HttpStreamJobTracker);
  };


  PassiveLogCollector();
  ~PassiveLogCollector();

  
  virtual void OnAddEntry(net::NetLog::EventType type,
                          const base::TimeTicks& time,
                          const net::NetLog::Source& source,
                          net::NetLog::EventPhase phase,
                          net::NetLog::EventParameters* params);

  
  void Clear();

  
  
  void GetAllCapturedEvents(ChromeNetLog::EntryList* out) const;

 private:
  
  SourceTrackerInterface* GetTrackerForSourceType(
      net::NetLog::SourceType source_type);

  FRIEND_TEST_ALL_PREFIXES(PassiveLogCollectorTest,
                           HoldReferenceToDependentSource);
  FRIEND_TEST_ALL_PREFIXES(PassiveLogCollectorTest,
                           HoldReferenceToDeletedSource);

  GlobalSourceTracker global_source_tracker_;
  ConnectJobTracker connect_job_tracker_;
  SocketTracker socket_tracker_;
  RequestTracker url_request_tracker_;
  RequestTracker socket_stream_tracker_;
  InitProxyResolverTracker init_proxy_resolver_tracker_;
  SpdySessionTracker spdy_session_tracker_;
  DNSRequestTracker dns_request_tracker_;
  DNSJobTracker dns_job_tracker_;
  DiskCacheEntryTracker disk_cache_entry_tracker_;
  MemCacheEntryTracker mem_cache_entry_tracker_;
  HttpStreamJobTracker http_stream_job_tracker_;

  
  
  
  SourceTrackerInterface* trackers_[net::NetLog::SOURCE_COUNT];

  
  
  uint32 num_events_seen_;

  DISALLOW_COPY_AND_ASSIGN(PassiveLogCollector);
};

#endif  
