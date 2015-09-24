// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_QUOTA_MOCK_QUOTA_MANAGER_H_
#define WEBKIT_BROWSER_QUOTA_MOCK_QUOTA_MANAGER_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "url/gurl.h"
#include "webkit/browser/quota/quota_client.h"
#include "webkit/browser/quota/quota_manager.h"
#include "webkit/browser/quota/quota_task.h"
#include "webkit/common/quota/quota_types.h"

namespace quota {

class MockQuotaManager : public QuotaManager {
 public:
  MockQuotaManager(bool is_incognito,
                   const base::FilePath& profile_path,
                   base::SingleThreadTaskRunner* io_thread,
                   base::SequencedTaskRunner* db_thread,
                   SpecialStoragePolicy* special_storage_policy);

  
  
  
  
  virtual void GetUsageAndQuota(
      const GURL& origin,
      quota::StorageType type,
      const GetUsageAndQuotaCallback& callback) OVERRIDE;

  
  
  
  virtual void GetOriginsModifiedSince(
      StorageType type,
      base::Time modified_since,
      const GetOriginsCallback& callback) OVERRIDE;

  
  
  
  
  
  
  virtual void DeleteOriginData(const GURL& origin,
                                StorageType type,
                                int quota_client_mask,
                                const StatusCallback& callback) OVERRIDE;

  
  void SetQuota(const GURL& origin, StorageType type, int64 quota);

  
  
  
  
  
  bool AddOrigin(const GURL& origin,
                 StorageType type,
                 int quota_client_mask,
                 base::Time modified);

  
  
  
  
  bool OriginHasData(const GURL& origin,
                     StorageType type,
                     QuotaClient::ID quota_client) const;

 protected:
  virtual ~MockQuotaManager();

 private:
  friend class MockQuotaManagerProxy;

  
  
  
  struct OriginInfo {
    OriginInfo(const GURL& origin,
               StorageType type,
               int quota_client_mask,
               base::Time modified);
    ~OriginInfo();

    GURL origin;
    StorageType type;
    int quota_client_mask;
    base::Time modified;
  };

  
  
  
  
  struct StorageInfo {
    StorageInfo();
    ~StorageInfo();
    int64 usage;
    int64 quota;
  };

  typedef std::pair<GURL, StorageType> OriginAndType;
  typedef std::map<OriginAndType, StorageInfo> UsageAndQuotaMap;

  
  void UpdateUsage(const GURL& origin, StorageType type, int64 delta);
  void DidGetModifiedSince(const GetOriginsCallback& callback,
                           std::set<GURL>* origins,
                           StorageType storage_type);
  void DidDeleteOriginData(const StatusCallback& callback,
                           QuotaStatusCode status);

  
  std::vector<OriginInfo> origins_;
  UsageAndQuotaMap usage_and_quota_map_;
  base::WeakPtrFactory<MockQuotaManager> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(MockQuotaManager);
};

class MockQuotaManagerProxy : public QuotaManagerProxy {
 public:
  
  MockQuotaManagerProxy(MockQuotaManager* quota_manager,
                        base::SingleThreadTaskRunner* task_runner);

  virtual void RegisterClient(QuotaClient* client) OVERRIDE;

  void SimulateQuotaManagerDestroyed();

  
  virtual void NotifyOriginInUse(const GURL& origin) OVERRIDE {}
  virtual void NotifyOriginNoLongerInUse(const GURL& origin) OVERRIDE {}
  virtual void SetUsageCacheEnabled(QuotaClient::ID client_id,
                                    const GURL& origin,
                                    StorageType type,
                                    bool enabled) OVERRIDE {}
  virtual void GetUsageAndQuota(
      base::SequencedTaskRunner* original_task_runner,
      const GURL& origin,
      StorageType type,
      const QuotaManager::GetUsageAndQuotaCallback& callback) OVERRIDE {}

  
  
  
  
  virtual void NotifyStorageAccessed(QuotaClient::ID client_id,
                                     const GURL& origin,
                                     StorageType type) OVERRIDE;

  
  
  
  
  virtual void NotifyStorageModified(QuotaClient::ID client_id,
                                     const GURL& origin,
                                     StorageType type,
                                     int64 delta) OVERRIDE;

  int notify_storage_accessed_count() const { return storage_accessed_count_; }
  int notify_storage_modified_count() const { return storage_modified_count_; }
  GURL last_notified_origin() const { return last_notified_origin_; }
  StorageType last_notified_type() const { return last_notified_type_; }
  int64 last_notified_delta() const { return last_notified_delta_; }

 protected:
  virtual ~MockQuotaManagerProxy();

 private:
  MockQuotaManager* mock_manager() const {
    return static_cast<MockQuotaManager*>(quota_manager());
  }

  int storage_accessed_count_;
  int storage_modified_count_;
  GURL last_notified_origin_;
  StorageType last_notified_type_;
  int64 last_notified_delta_;

  QuotaClient* registered_client_;
};

}  

#endif  
