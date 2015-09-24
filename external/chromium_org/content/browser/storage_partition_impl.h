// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_STORAGE_PARTITION_IMPL_H_
#define CONTENT_BROWSER_STORAGE_PARTITION_IMPL_H_

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "content/browser/appcache/chrome_appcache_service.h"
#include "content/browser/dom_storage/dom_storage_context_wrapper.h"
#include "content/browser/indexed_db/indexed_db_context_impl.h"
#include "content/browser/media/webrtc_identity_store.h"
#include "content/browser/service_worker/service_worker_context_wrapper.h"
#include "content/common/content_export.h"
#include "content/public/browser/storage_partition.h"
#include "webkit/browser/quota/special_storage_policy.h"

namespace content {

class StoragePartitionImpl : public StoragePartition {
 public:
  CONTENT_EXPORT virtual ~StoragePartitionImpl();

  
  
  CONTENT_EXPORT static int GenerateQuotaClientMask(uint32 remove_mask);

  CONTENT_EXPORT void OverrideQuotaManagerForTesting(
      quota::QuotaManager* quota_manager);
  CONTENT_EXPORT void OverrideSpecialStoragePolicyForTesting(
      quota::SpecialStoragePolicy* special_storage_policy);

  
  virtual base::FilePath GetPath() OVERRIDE;
  virtual net::URLRequestContextGetter* GetURLRequestContext() OVERRIDE;
  virtual net::URLRequestContextGetter* GetMediaURLRequestContext() OVERRIDE;
  virtual quota::QuotaManager* GetQuotaManager() OVERRIDE;
  virtual ChromeAppCacheService* GetAppCacheService() OVERRIDE;
  virtual fileapi::FileSystemContext* GetFileSystemContext() OVERRIDE;
  virtual webkit_database::DatabaseTracker* GetDatabaseTracker() OVERRIDE;
  virtual DOMStorageContextWrapper* GetDOMStorageContext() OVERRIDE;
  virtual IndexedDBContextImpl* GetIndexedDBContext() OVERRIDE;

  virtual void ClearDataForOrigin(
      uint32 remove_mask,
      uint32 quota_storage_remove_mask,
      const GURL& storage_origin,
      net::URLRequestContextGetter* request_context_getter) OVERRIDE;
  virtual void ClearData(uint32 remove_mask,
                         uint32 quota_storage_remove_mask,
                         const GURL* storage_origin,
                         const OriginMatcherFunction& origin_matcher,
                         const base::Time begin,
                         const base::Time end,
                         const base::Closure& callback) OVERRIDE;

  ServiceWorkerContextWrapper* GetServiceWorkerContext();

  WebRTCIdentityStore* GetWebRTCIdentityStore();

  struct DataDeletionHelper;
  struct QuotaManagedDataDeletionHelper;

 private:
  friend class StoragePartitionImplMap;
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionShaderClearTest, ClearShaderCache);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveQuotaManagedDataForeverBoth);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveQuotaManagedDataForeverOnlyTemporary);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveQuotaManagedDataForeverOnlyPersistent);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveQuotaManagedDataForeverNeither);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveQuotaManagedDataForeverSpecificOrigin);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveQuotaManagedDataForLastHour);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveQuotaManagedDataForLastWeek);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveQuotaManagedUnprotectedOrigins);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveQuotaManagedProtectedSpecificOrigin);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveQuotaManagedProtectedOrigins);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveQuotaManagedIgnoreDevTools);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest, RemoveCookieForever);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest, RemoveCookieLastHour);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveUnprotectedLocalStorageForever);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveProtectedLocalStorageForever);
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionImplTest,
                           RemoveLocalStorageForLastWeek);

  
  
  
  
  
  
  static StoragePartitionImpl* Create(BrowserContext* context,
                                      bool in_memory,
                                      const base::FilePath& profile_path);

  CONTENT_EXPORT StoragePartitionImpl(
      const base::FilePath& partition_path,
      quota::QuotaManager* quota_manager,
      ChromeAppCacheService* appcache_service,
      fileapi::FileSystemContext* filesystem_context,
      webkit_database::DatabaseTracker* database_tracker,
      DOMStorageContextWrapper* dom_storage_context,
      IndexedDBContextImpl* indexed_db_context,
      ServiceWorkerContextWrapper* service_worker_context,
      WebRTCIdentityStore* webrtc_identity_store,
      quota::SpecialStoragePolicy* special_storage_policy);

  void ClearDataImpl(uint32 remove_mask,
                     uint32 quota_storage_remove_mask,
                     const GURL* remove_origin,
                     const OriginMatcherFunction& origin_matcher,
                     net::URLRequestContextGetter* rq_context,
                     const base::Time begin,
                     const base::Time end,
                     const base::Closure& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  CONTENT_EXPORT void SetURLRequestContext(
      net::URLRequestContextGetter* url_request_context);
  void SetMediaURLRequestContext(
      net::URLRequestContextGetter* media_url_request_context);

  base::FilePath partition_path_;
  scoped_refptr<net::URLRequestContextGetter> url_request_context_;
  scoped_refptr<net::URLRequestContextGetter> media_url_request_context_;
  scoped_refptr<quota::QuotaManager> quota_manager_;
  scoped_refptr<ChromeAppCacheService> appcache_service_;
  scoped_refptr<fileapi::FileSystemContext> filesystem_context_;
  scoped_refptr<webkit_database::DatabaseTracker> database_tracker_;
  scoped_refptr<DOMStorageContextWrapper> dom_storage_context_;
  scoped_refptr<IndexedDBContextImpl> indexed_db_context_;
  scoped_refptr<ServiceWorkerContextWrapper> service_worker_context_;
  scoped_refptr<WebRTCIdentityStore> webrtc_identity_store_;
  scoped_refptr<quota::SpecialStoragePolicy> special_storage_policy_;

  DISALLOW_COPY_AND_ASSIGN(StoragePartitionImpl);
};

}  

#endif  
