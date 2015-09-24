// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_QUOTA_QUOTA_MANAGER_H_
#define WEBKIT_BROWSER_QUOTA_QUOTA_MANAGER_H_

#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "webkit/browser/quota/quota_callbacks.h"
#include "webkit/browser/quota/quota_client.h"
#include "webkit/browser/quota/quota_database.h"
#include "webkit/browser/quota/quota_task.h"
#include "webkit/browser/quota/special_storage_policy.h"
#include "webkit/browser/webkit_storage_browser_export.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
class SingleThreadTaskRunner;
}

namespace quota_internals {
class QuotaInternalsProxy;
}

namespace quota {

class MockQuotaManager;
class QuotaDatabase;
class QuotaManagerProxy;
class QuotaTemporaryStorageEvictor;
class UsageTracker;

struct QuotaManagerDeleter;

struct WEBKIT_STORAGE_BROWSER_EXPORT UsageAndQuota {
  int64 usage;
  int64 global_limited_usage;
  int64 quota;
  int64 available_disk_space;

  UsageAndQuota();
  UsageAndQuota(int64 usage,
                int64 global_limited_usage,
                int64 quota,
                int64 available_disk_space);
};

class WEBKIT_STORAGE_BROWSER_EXPORT QuotaEvictionHandler {
 public:
  typedef base::Callback<void(const GURL&)> GetLRUOriginCallback;
  typedef StatusCallback EvictOriginDataCallback;
  typedef base::Callback<void(QuotaStatusCode status,
                              const UsageAndQuota& usage_and_quota)>
      UsageAndQuotaCallback;

  
  
  virtual void GetLRUOrigin(
      StorageType type,
      const GetLRUOriginCallback& callback) = 0;

  virtual void EvictOriginData(
      const GURL& origin,
      StorageType type,
      const EvictOriginDataCallback& callback) = 0;

  virtual void GetUsageAndQuotaForEviction(
      const UsageAndQuotaCallback& callback) = 0;

 protected:
  virtual ~QuotaEvictionHandler() {}
};

struct UsageInfo {
  UsageInfo(const std::string& host, StorageType type, int64 usage)
      : host(host),
        type(type),
        usage(usage) {}
  std::string host;
  StorageType type;
  int64 usage;
};

class WEBKIT_STORAGE_BROWSER_EXPORT QuotaManager
    : public QuotaTaskObserver,
      public QuotaEvictionHandler,
      public base::RefCountedThreadSafe<QuotaManager, QuotaManagerDeleter> {
 public:
  typedef base::Callback<void(QuotaStatusCode,
                              int64 ,
                              int64 )>
      GetUsageAndQuotaCallback;

  static const int64 kIncognitoDefaultQuotaLimit;
  static const int64 kNoLimit;

  QuotaManager(bool is_incognito,
               const base::FilePath& profile_path,
               base::SingleThreadTaskRunner* io_thread,
               base::SequencedTaskRunner* db_thread,
               SpecialStoragePolicy* special_storage_policy);

  
  QuotaManagerProxy* proxy() { return proxy_.get(); }

  
  void GetUsageInfo(const GetUsageInfoCallback& callback);

  
  
  virtual void GetUsageAndQuotaForWebApps(
      const GURL& origin,
      StorageType type,
      const GetUsageAndQuotaCallback& callback);

  
  
  
  
  
  
  virtual void GetUsageAndQuota(
      const GURL& origin,
      StorageType type,
      const GetUsageAndQuotaCallback& callback);

  
  
  
  void NotifyStorageAccessed(QuotaClient::ID client_id,
                             const GURL& origin,
                             StorageType type);

  
  
  
  void NotifyStorageModified(QuotaClient::ID client_id,
                             const GURL& origin,
                             StorageType type,
                             int64 delta);

  
  
  
  void NotifyOriginInUse(const GURL& origin);
  void NotifyOriginNoLongerInUse(const GURL& origin);
  bool IsOriginInUse(const GURL& origin) const {
    return origins_in_use_.find(origin) != origins_in_use_.end();
  }

  void SetUsageCacheEnabled(QuotaClient::ID client_id,
                            const GURL& origin,
                            StorageType type,
                            bool enabled);

  
  
  
  
  
  
  
  virtual void DeleteOriginData(const GURL& origin,
                                StorageType type,
                                int quota_client_mask,
                                const StatusCallback& callback);
  void DeleteHostData(const std::string& host,
                      StorageType type,
                      int quota_client_mask,
                      const StatusCallback& callback);

  
  void GetAvailableSpace(const AvailableSpaceCallback& callback);
  void GetTemporaryGlobalQuota(const QuotaCallback& callback);

  
  void SetTemporaryGlobalOverrideQuota(int64 new_quota,
                                       const QuotaCallback& callback);

  void GetPersistentHostQuota(const std::string& host,
                              const QuotaCallback& callback);
  void SetPersistentHostQuota(const std::string& host,
                              int64 new_quota,
                              const QuotaCallback& callback);
  void GetGlobalUsage(StorageType type, const GlobalUsageCallback& callback);
  void GetHostUsage(const std::string& host, StorageType type,
                    const UsageCallback& callback);
  void GetHostUsage(const std::string& host, StorageType type,
                    QuotaClient::ID client_id,
                    const UsageCallback& callback);

  bool IsTrackingHostUsage(StorageType type, QuotaClient::ID client_id) const;

  void GetStatistics(std::map<std::string, std::string>* statistics);

  bool IsStorageUnlimited(const GURL& origin, StorageType type) const;

  bool CanQueryDiskSize(const GURL& origin) const {
    return special_storage_policy_.get() &&
           special_storage_policy_->CanQueryDiskSize(origin);
  }

  virtual void GetOriginsModifiedSince(StorageType type,
                                       base::Time modified_since,
                                       const GetOriginsCallback& callback);

  bool ResetUsageTracker(StorageType type);

  
  
  static const int kPerHostTemporaryPortion;

  static const char kDatabaseName[];

  static const int64 kMinimumPreserveForSystem;

  static const int kThresholdOfErrorsToBeBlacklisted;

  static const int kEvictionIntervalInMilliSeconds;

  
  
  
  static int64 kSyncableStorageDefaultHostQuota;

 protected:
  virtual ~QuotaManager();

 private:
  friend class base::DeleteHelper<QuotaManager>;
  friend class base::RefCountedThreadSafe<QuotaManager, QuotaManagerDeleter>;
  friend class MockQuotaManager;
  friend class MockStorageClient;
  friend class quota_internals::QuotaInternalsProxy;
  friend class QuotaManagerProxy;
  friend class QuotaManagerTest;
  friend class QuotaTemporaryStorageEvictor;
  friend struct QuotaManagerDeleter;

  class GetUsageInfoTask;

  class OriginDataDeleter;
  class HostDataDeleter;

  class GetModifiedSinceHelper;
  class DumpQuotaTableHelper;
  class DumpOriginInfoTableHelper;

  typedef QuotaDatabase::QuotaTableEntry QuotaTableEntry;
  typedef QuotaDatabase::OriginInfoTableEntry OriginInfoTableEntry;
  typedef std::vector<QuotaTableEntry> QuotaTableEntries;
  typedef std::vector<OriginInfoTableEntry> OriginInfoTableEntries;

  
  
  typedef int64 (*GetAvailableDiskSpaceFn)(const base::FilePath&);

  typedef base::Callback<void(const QuotaTableEntries&)>
      DumpQuotaTableCallback;
  typedef base::Callback<void(const OriginInfoTableEntries&)>
      DumpOriginInfoTableCallback;

  struct EvictionContext {
    EvictionContext();
    virtual ~EvictionContext();
    GURL evicted_origin;
    StorageType evicted_type;

    EvictOriginDataCallback evict_origin_data_callback;
  };

  typedef QuotaEvictionHandler::UsageAndQuotaCallback
      UsageAndQuotaDispatcherCallback;

  
  
  
  
  void LazyInitialize();

  
  
  
  void RegisterClient(QuotaClient* client);

  UsageTracker* GetUsageTracker(StorageType type) const;

  
  
  void GetCachedOrigins(StorageType type, std::set<GURL>* origins);

  
  void NotifyStorageAccessedInternal(
      QuotaClient::ID client_id,
      const GURL& origin,
      StorageType type,
      base::Time accessed_time);
  void NotifyStorageModifiedInternal(
      QuotaClient::ID client_id,
      const GURL& origin,
      StorageType type,
      int64 delta,
      base::Time modified_time);

  void DumpQuotaTable(const DumpQuotaTableCallback& callback);
  void DumpOriginInfoTable(const DumpOriginInfoTableCallback& callback);

  
  void StartEviction();
  void DeleteOriginFromDatabase(const GURL& origin, StorageType type);

  void DidOriginDataEvicted(QuotaStatusCode status);

  void ReportHistogram();
  void DidGetTemporaryGlobalUsageForHistogram(int64 usage,
                                              int64 unlimited_usage);
  void DidGetPersistentGlobalUsageForHistogram(int64 usage,
                                               int64 unlimited_usage);

  
  virtual void GetLRUOrigin(
      StorageType type,
      const GetLRUOriginCallback& callback) OVERRIDE;
  virtual void EvictOriginData(
      const GURL& origin,
      StorageType type,
      const EvictOriginDataCallback& callback) OVERRIDE;
  virtual void GetUsageAndQuotaForEviction(
      const UsageAndQuotaCallback& callback) OVERRIDE;

  void DidSetTemporaryGlobalOverrideQuota(const QuotaCallback& callback,
                                          const int64* new_quota,
                                          bool success);
  void DidGetPersistentHostQuota(const std::string& host,
                                 const int64* quota,
                                 bool success);
  void DidSetPersistentHostQuota(const std::string& host,
                                 const QuotaCallback& callback,
                                 const int64* new_quota,
                                 bool success);
  void DidInitialize(int64* temporary_quota_override,
                     int64* desired_available_space,
                     bool success);
  void DidGetLRUOrigin(const GURL* origin,
                       bool success);
  void DidGetInitialTemporaryGlobalQuota(QuotaStatusCode status,
                                         int64 quota_unused);
  void DidInitializeTemporaryOriginsInfo(bool success);
  void DidGetAvailableSpace(int64 space);
  void DidDatabaseWork(bool success);

  void DeleteOnCorrectThread() const;

  void PostTaskAndReplyWithResultForDBThread(
      const tracked_objects::Location& from_here,
      const base::Callback<bool(QuotaDatabase*)>& task,
      const base::Callback<void(bool)>& reply);

  const bool is_incognito_;
  const base::FilePath profile_path_;

  scoped_refptr<QuotaManagerProxy> proxy_;
  bool db_disabled_;
  bool eviction_disabled_;
  scoped_refptr<base::SingleThreadTaskRunner> io_thread_;
  scoped_refptr<base::SequencedTaskRunner> db_thread_;
  mutable scoped_ptr<QuotaDatabase> database_;

  GetLRUOriginCallback lru_origin_callback_;
  std::set<GURL> access_notified_origins_;

  QuotaClientList clients_;

  scoped_ptr<UsageTracker> temporary_usage_tracker_;
  scoped_ptr<UsageTracker> persistent_usage_tracker_;
  scoped_ptr<UsageTracker> syncable_usage_tracker_;
  
  

  scoped_ptr<QuotaTemporaryStorageEvictor> temporary_storage_evictor_;
  EvictionContext eviction_context_;

  ClosureQueue db_initialization_callbacks_;
  AvailableSpaceCallbackQueue available_space_callbacks_;
  GlobalQuotaCallbackQueue temporary_global_quota_callbacks_;
  HostQuotaCallbackMap persistent_host_quota_callbacks_;

  bool temporary_quota_initialized_;
  int64 temporary_quota_override_;

  int64 desired_available_space_;

  
  std::map<GURL, int> origins_in_use_;
  
  std::map<GURL, int> origins_in_error_;

  scoped_refptr<SpecialStoragePolicy> special_storage_policy_;

  base::RepeatingTimer<QuotaManager> histogram_timer_;

  
  // overwritten by QuotaManagerTest in order to attain a deterministic reported
  
  GetAvailableDiskSpaceFn get_disk_space_fn_;

  base::WeakPtrFactory<QuotaManager> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(QuotaManager);
};

struct QuotaManagerDeleter {
  static void Destruct(const QuotaManager* manager) {
    manager->DeleteOnCorrectThread();
  }
};

class WEBKIT_STORAGE_BROWSER_EXPORT QuotaManagerProxy
    : public base::RefCountedThreadSafe<QuotaManagerProxy> {
 public:
  typedef QuotaManager::GetUsageAndQuotaCallback
      GetUsageAndQuotaCallback;

  virtual void RegisterClient(QuotaClient* client);
  virtual void NotifyStorageAccessed(QuotaClient::ID client_id,
                                     const GURL& origin,
                                     StorageType type);
  virtual void NotifyStorageModified(QuotaClient::ID client_id,
                                     const GURL& origin,
                                     StorageType type,
                                     int64 delta);
  virtual void NotifyOriginInUse(const GURL& origin);
  virtual void NotifyOriginNoLongerInUse(const GURL& origin);

  virtual void SetUsageCacheEnabled(QuotaClient::ID client_id,
                                    const GURL& origin,
                                    StorageType type,
                                    bool enabled);
  virtual void GetUsageAndQuota(
      base::SequencedTaskRunner* original_task_runner,
      const GURL& origin,
      StorageType type,
      const GetUsageAndQuotaCallback& callback);

  
  
  QuotaManager* quota_manager() const;

 protected:
  friend class QuotaManager;
  friend class base::RefCountedThreadSafe<QuotaManagerProxy>;

  QuotaManagerProxy(QuotaManager* manager,
                    base::SingleThreadTaskRunner* io_thread);
  virtual ~QuotaManagerProxy();

  QuotaManager* manager_;  
  scoped_refptr<base::SingleThreadTaskRunner> io_thread_;

  DISALLOW_COPY_AND_ASSIGN(QuotaManagerProxy);
};

}  

#endif  
