// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_COOKIES_TREE_MODEL_H_
#define CHROME_BROWSER_BROWSING_DATA_COOKIES_TREE_MODEL_H_


#include <list>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/browsing_data/browsing_data_appcache_helper.h"
#include "chrome/browser/browsing_data/browsing_data_database_helper.h"
#include "chrome/browser/browsing_data/browsing_data_file_system_helper.h"
#include "chrome/browser/browsing_data/browsing_data_indexed_db_helper.h"
#include "chrome/browser/browsing_data/browsing_data_local_storage_helper.h"
#include "chrome/browser/browsing_data/browsing_data_quota_helper.h"
#include "chrome/browser/browsing_data/local_data_container.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/extensions/extension_set.h"
#include "net/ssl/server_bound_cert_store.h"
#include "ui/base/models/tree_node_model.h"

class BrowsingDataCookieHelper;
class BrowsingDataServerBoundCertHelper;
class CookieSettings;
class CookiesTreeModel;
class CookieTreeAppCacheNode;
class CookieTreeAppCachesNode;
class CookieTreeCookieNode;
class CookieTreeCookiesNode;
class CookieTreeDatabaseNode;
class CookieTreeDatabasesNode;
class CookieTreeFileSystemNode;
class CookieTreeFileSystemsNode;
class CookieTreeFlashLSONode;
class CookieTreeHostNode;
class CookieTreeIndexedDBNode;
class CookieTreeIndexedDBsNode;
class CookieTreeLocalStorageNode;
class CookieTreeLocalStoragesNode;
class CookieTreeQuotaNode;
class CookieTreeServerBoundCertNode;
class CookieTreeServerBoundCertsNode;
class CookieTreeSessionStorageNode;
class CookieTreeSessionStoragesNode;
class ExtensionSpecialStoragePolicy;

namespace net {
class CanonicalCookie;
}

class CookieTreeNode : public ui::TreeNode<CookieTreeNode> {
 public:
  
  
  struct DetailedInfo {
    
    enum NodeType {
      TYPE_NONE,
      TYPE_ROOT,  
      TYPE_HOST,  
      TYPE_COOKIES,  
      TYPE_COOKIE,  
      TYPE_DATABASES,  
      TYPE_DATABASE,  
      TYPE_LOCAL_STORAGES,  
      TYPE_LOCAL_STORAGE,  
      TYPE_SESSION_STORAGES,  
      TYPE_SESSION_STORAGE,  
      TYPE_APPCACHES,  
      TYPE_APPCACHE,  
      TYPE_INDEXED_DBS,  
      TYPE_INDEXED_DB,  
      TYPE_FILE_SYSTEMS,  
      TYPE_FILE_SYSTEM,  
      TYPE_QUOTA,  
      TYPE_SERVER_BOUND_CERTS, 
      TYPE_SERVER_BOUND_CERT, 
      TYPE_FLASH_LSO,  
    };

    DetailedInfo();
    ~DetailedInfo();

    DetailedInfo& Init(NodeType type);
    DetailedInfo& InitHost();
    DetailedInfo& InitCookie(const net::CanonicalCookie* cookie);
    DetailedInfo& InitDatabase(
        const BrowsingDataDatabaseHelper::DatabaseInfo* database_info);
    DetailedInfo& InitLocalStorage(
        const BrowsingDataLocalStorageHelper::LocalStorageInfo*
        local_storage_info);
    DetailedInfo& InitSessionStorage(
        const BrowsingDataLocalStorageHelper::LocalStorageInfo*
        session_storage_info);
    DetailedInfo& InitAppCache(const GURL& origin,
                               const appcache::AppCacheInfo* appcache_info);
    DetailedInfo& InitIndexedDB(
        const content::IndexedDBInfo* indexed_db_info);
    DetailedInfo& InitFileSystem(
        const BrowsingDataFileSystemHelper::FileSystemInfo* file_system_info);
    DetailedInfo& InitQuota(
        const BrowsingDataQuotaHelper::QuotaInfo* quota_info);
    DetailedInfo& InitServerBoundCert(
        const net::ServerBoundCertStore::ServerBoundCert* server_bound_cert);
    DetailedInfo& InitFlashLSO(const std::string& flash_lso_domain);

    NodeType node_type;
    GURL origin;
    const net::CanonicalCookie* cookie;
    const BrowsingDataDatabaseHelper::DatabaseInfo* database_info;
    const BrowsingDataLocalStorageHelper::LocalStorageInfo* local_storage_info;
    const BrowsingDataLocalStorageHelper::LocalStorageInfo*
        session_storage_info;
    const appcache::AppCacheInfo* appcache_info;
    const content::IndexedDBInfo* indexed_db_info;
    const BrowsingDataFileSystemHelper::FileSystemInfo* file_system_info;
    const BrowsingDataQuotaHelper::QuotaInfo* quota_info;
    const net::ServerBoundCertStore::ServerBoundCert* server_bound_cert;
    std::string flash_lso_domain;
  };

  CookieTreeNode() {}
  explicit CookieTreeNode(const base::string16& title)
      : ui::TreeNode<CookieTreeNode>(title) {}
  virtual ~CookieTreeNode() {}

  
  
  virtual void DeleteStoredObjects();

  
  virtual CookiesTreeModel* GetModel() const;

  
  
  virtual DetailedInfo GetDetailedInfo() const = 0;

 protected:
  void AddChildSortedByTitle(CookieTreeNode* new_child);

 private:
  DISALLOW_COPY_AND_ASSIGN(CookieTreeNode);
};

class CookieTreeRootNode : public CookieTreeNode {
 public:
  explicit CookieTreeRootNode(CookiesTreeModel* model);
  virtual ~CookieTreeRootNode();

  CookieTreeHostNode* GetOrCreateHostNode(const GURL& url);

  
  virtual CookiesTreeModel* GetModel() const OVERRIDE;
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

 private:
  CookiesTreeModel* model_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeRootNode);
};

class CookieTreeHostNode : public CookieTreeNode {
 public:
  
  static base::string16 TitleForUrl(const GURL& url);

  explicit CookieTreeHostNode(const GURL& url);
  virtual ~CookieTreeHostNode();

  
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

  
  CookieTreeCookiesNode* GetOrCreateCookiesNode();
  CookieTreeDatabasesNode* GetOrCreateDatabasesNode();
  CookieTreeLocalStoragesNode* GetOrCreateLocalStoragesNode();
  CookieTreeSessionStoragesNode* GetOrCreateSessionStoragesNode();
  CookieTreeAppCachesNode* GetOrCreateAppCachesNode();
  CookieTreeIndexedDBsNode* GetOrCreateIndexedDBsNode();
  CookieTreeFileSystemsNode* GetOrCreateFileSystemsNode();
  CookieTreeServerBoundCertsNode* GetOrCreateServerBoundCertsNode();
  CookieTreeQuotaNode* UpdateOrCreateQuotaNode(
      std::list<BrowsingDataQuotaHelper::QuotaInfo>::iterator quota_info);
  CookieTreeFlashLSONode* GetOrCreateFlashLSONode(const std::string& domain);

  std::string canonicalized_host() const { return canonicalized_host_; }

  
  
  void CreateContentException(CookieSettings* cookie_settings,
                              ContentSetting setting) const;

  
  bool CanCreateContentException() const;

  const std::string GetHost() const;

 private:
  
  
  
  
  
  CookieTreeCookiesNode* cookies_child_;
  CookieTreeDatabasesNode* databases_child_;
  CookieTreeLocalStoragesNode* local_storages_child_;
  CookieTreeSessionStoragesNode* session_storages_child_;
  CookieTreeAppCachesNode* appcaches_child_;
  CookieTreeIndexedDBsNode* indexed_dbs_child_;
  CookieTreeFileSystemsNode* file_systems_child_;
  CookieTreeQuotaNode* quota_child_;
  CookieTreeServerBoundCertsNode* server_bound_certs_child_;
  CookieTreeFlashLSONode* flash_lso_child_;

  
  GURL url_;

  std::string canonicalized_host_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeHostNode);
};

class CookieTreeCookieNode : public CookieTreeNode {
 public:
  friend class CookieTreeCookiesNode;

  
  
  explicit CookieTreeCookieNode(
      std::list<net::CanonicalCookie>::iterator cookie);
  virtual ~CookieTreeCookieNode();

  
  virtual void DeleteStoredObjects() OVERRIDE;
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

 private:
  
  
  std::list<net::CanonicalCookie>::iterator cookie_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeCookieNode);
};

class CookieTreeCookiesNode : public CookieTreeNode {
 public:
  CookieTreeCookiesNode();
  virtual ~CookieTreeCookiesNode();

  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

  void AddCookieNode(CookieTreeCookieNode* child) {
    AddChildSortedByTitle(child);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CookieTreeCookiesNode);
};

class CookieTreeAppCacheNode : public CookieTreeNode {
 public:
  friend class CookieTreeAppCachesNode;

  
  
  explicit CookieTreeAppCacheNode(
      const GURL& origin_url,
      std::list<appcache::AppCacheInfo>::iterator appcache_info);
  virtual ~CookieTreeAppCacheNode();

  virtual void DeleteStoredObjects() OVERRIDE;
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

 private:
  GURL origin_url_;
  std::list<appcache::AppCacheInfo>::iterator appcache_info_;
  DISALLOW_COPY_AND_ASSIGN(CookieTreeAppCacheNode);
};

class CookieTreeAppCachesNode : public CookieTreeNode {
 public:
  CookieTreeAppCachesNode();
  virtual ~CookieTreeAppCachesNode();

  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

  void AddAppCacheNode(CookieTreeAppCacheNode* child) {
    AddChildSortedByTitle(child);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CookieTreeAppCachesNode);
};

class CookieTreeDatabaseNode : public CookieTreeNode {
 public:
  friend class CookieTreeDatabasesNode;

  
  
  explicit CookieTreeDatabaseNode(
      std::list<BrowsingDataDatabaseHelper::DatabaseInfo>::iterator
          database_info);
  virtual ~CookieTreeDatabaseNode();

  virtual void DeleteStoredObjects() OVERRIDE;
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

 private:
  
  
  std::list<BrowsingDataDatabaseHelper::DatabaseInfo>::iterator
      database_info_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeDatabaseNode);
};

class CookieTreeDatabasesNode : public CookieTreeNode {
 public:
  CookieTreeDatabasesNode();
  virtual ~CookieTreeDatabasesNode();

  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

  void AddDatabaseNode(CookieTreeDatabaseNode* child) {
    AddChildSortedByTitle(child);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CookieTreeDatabasesNode);
};

class CookieTreeFileSystemNode : public CookieTreeNode {
 public:
  friend class CookieTreeFileSystemsNode;

  
  
  explicit CookieTreeFileSystemNode(
      std::list<BrowsingDataFileSystemHelper::FileSystemInfo>::iterator
          file_system_info);
  virtual ~CookieTreeFileSystemNode();

  virtual void DeleteStoredObjects() OVERRIDE;
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

 private:
  
  
  std::list<BrowsingDataFileSystemHelper::FileSystemInfo>::iterator
      file_system_info_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeFileSystemNode);
};

class CookieTreeFileSystemsNode : public CookieTreeNode {
 public:
  CookieTreeFileSystemsNode();
  virtual ~CookieTreeFileSystemsNode();

  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

  void AddFileSystemNode(CookieTreeFileSystemNode* child) {
    AddChildSortedByTitle(child);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CookieTreeFileSystemsNode);
};

class CookieTreeLocalStorageNode : public CookieTreeNode {
 public:
  
  
  explicit CookieTreeLocalStorageNode(
      std::list<BrowsingDataLocalStorageHelper::LocalStorageInfo>::iterator
          local_storage_info);
  virtual ~CookieTreeLocalStorageNode();

  
  virtual void DeleteStoredObjects() OVERRIDE;
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

 private:
  
  
  std::list<BrowsingDataLocalStorageHelper::LocalStorageInfo>::iterator
      local_storage_info_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeLocalStorageNode);
};

class CookieTreeLocalStoragesNode : public CookieTreeNode {
 public:
  CookieTreeLocalStoragesNode();
  virtual ~CookieTreeLocalStoragesNode();

  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

  void AddLocalStorageNode(CookieTreeLocalStorageNode* child) {
    AddChildSortedByTitle(child);
  }

 private:

  DISALLOW_COPY_AND_ASSIGN(CookieTreeLocalStoragesNode);
};


class CookieTreeSessionStorageNode : public CookieTreeNode {
 public:
  
  
  explicit CookieTreeSessionStorageNode(
      std::list<BrowsingDataLocalStorageHelper::LocalStorageInfo>::iterator
          session_storage_info);
  virtual ~CookieTreeSessionStorageNode();

  
  virtual void DeleteStoredObjects() OVERRIDE;
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

 private:
  
  
  std::list<BrowsingDataLocalStorageHelper::LocalStorageInfo>::iterator
      session_storage_info_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeSessionStorageNode);
};

class CookieTreeSessionStoragesNode : public CookieTreeNode {
 public:
  CookieTreeSessionStoragesNode();
  virtual ~CookieTreeSessionStoragesNode();

  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

  void AddSessionStorageNode(CookieTreeSessionStorageNode* child) {
    AddChildSortedByTitle(child);
  }

 private:

  DISALLOW_COPY_AND_ASSIGN(CookieTreeSessionStoragesNode);
};

class CookieTreeIndexedDBNode : public CookieTreeNode {
 public:
  
  
  explicit CookieTreeIndexedDBNode(
      std::list<content::IndexedDBInfo>::iterator
          indexed_db_info);
  virtual ~CookieTreeIndexedDBNode();

  
  virtual void DeleteStoredObjects() OVERRIDE;
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

 private:
  
  
  std::list<content::IndexedDBInfo>::iterator
      indexed_db_info_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeIndexedDBNode);
};

class CookieTreeIndexedDBsNode : public CookieTreeNode {
 public:
  CookieTreeIndexedDBsNode();
  virtual ~CookieTreeIndexedDBsNode();

  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

  void AddIndexedDBNode(CookieTreeIndexedDBNode* child) {
    AddChildSortedByTitle(child);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CookieTreeIndexedDBsNode);
};

class CookieTreeQuotaNode : public CookieTreeNode {
 public:
  
  
  explicit CookieTreeQuotaNode(
      std::list<BrowsingDataQuotaHelper::QuotaInfo>::iterator quota_info);
  virtual ~CookieTreeQuotaNode();

  virtual void DeleteStoredObjects() OVERRIDE;
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

 private:
  
  
  std::list<BrowsingDataQuotaHelper::QuotaInfo>::iterator quota_info_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeQuotaNode);
};

class CookieTreeServerBoundCertNode : public CookieTreeNode {
 public:
  friend class CookieTreeServerBoundCertsNode;

  
  
  explicit CookieTreeServerBoundCertNode(
      net::ServerBoundCertStore::ServerBoundCertList::iterator cert);
  virtual ~CookieTreeServerBoundCertNode();

  
  virtual void DeleteStoredObjects() OVERRIDE;
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

 private:
  
  
  net::ServerBoundCertStore::ServerBoundCertList::iterator server_bound_cert_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeServerBoundCertNode);
};

class CookieTreeServerBoundCertsNode : public CookieTreeNode {
 public:
  CookieTreeServerBoundCertsNode();
  virtual ~CookieTreeServerBoundCertsNode();

  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

  void AddServerBoundCertNode(CookieTreeServerBoundCertNode* child) {
    AddChildSortedByTitle(child);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CookieTreeServerBoundCertsNode);
};

class CookieTreeFlashLSONode : public CookieTreeNode {
 public:
  explicit CookieTreeFlashLSONode(const std::string& domain);
  virtual ~CookieTreeFlashLSONode();

  
  virtual void DeleteStoredObjects() OVERRIDE;
  virtual DetailedInfo GetDetailedInfo() const OVERRIDE;

 private:
  std::string domain_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeFlashLSONode);
};

class CookiesTreeModel : public ui::TreeNodeModel<CookieTreeNode> {
 public:
  CookiesTreeModel(LocalDataContainer* data_container,
                   ExtensionSpecialStoragePolicy* special_storage_policy,
                   bool group_by_cookie_source);
  virtual ~CookiesTreeModel();

  
  
  
  
  
  class Observer : public ui::TreeModelObserver {
   public:
    virtual void TreeModelBeginBatch(CookiesTreeModel* model) {}
    virtual void TreeModelEndBatch(CookiesTreeModel* model) {}
  };

  
  
  
  class ScopedBatchUpdateNotifier {
   public:
    ScopedBatchUpdateNotifier(CookiesTreeModel* model,
                              CookieTreeNode* node);
    ~ScopedBatchUpdateNotifier();

    void StartBatchUpdate();

   private:
    CookiesTreeModel* model_;
    CookieTreeNode* node_;
    bool batch_in_progress_;
  };

  
  
  
  virtual void GetIcons(std::vector<gfx::ImageSkia>* icons) OVERRIDE;

  
  
  
  virtual int GetIconIndex(ui::TreeModelNode* node) OVERRIDE;

  
  void DeleteAllStoredObjects();

  
  
  void DeleteCookieNode(CookieTreeNode* cookie_node);

  
  void UpdateSearchResults(const base::string16& filter);

  
  
  
  
  const ExtensionSet* ExtensionsProtectingNode(
      const CookieTreeNode& cookie_node);

  
  
  
  
  virtual void AddCookiesTreeObserver(Observer* observer);
  virtual void RemoveCookiesTreeObserver(Observer* observer);

  
  
  void PopulateAppCacheInfo(LocalDataContainer* container);
  void PopulateCookieInfo(LocalDataContainer* container);
  void PopulateDatabaseInfo(LocalDataContainer* container);
  void PopulateLocalStorageInfo(LocalDataContainer* container);
  void PopulateSessionStorageInfo(LocalDataContainer* container);
  void PopulateIndexedDBInfo(LocalDataContainer* container);
  void PopulateFileSystemInfo(LocalDataContainer* container);
  void PopulateQuotaInfo(LocalDataContainer* container);
  void PopulateServerBoundCertInfo(LocalDataContainer* container);
  void PopulateFlashLSOInfo(LocalDataContainer* container);

  BrowsingDataCookieHelper* GetCookieHelper(const std::string& app_id);
  LocalDataContainer* data_container() {
    return data_container_.get();
  }

 private:
  enum CookieIconIndex {
    ORIGIN = 0,
    COOKIE = 1,
    DATABASE = 2
  };

  void NotifyObserverBeginBatch();
  void NotifyObserverEndBatch();

  void PopulateAppCacheInfoWithFilter(LocalDataContainer* container,
                                      ScopedBatchUpdateNotifier* notifier,
                                      const base::string16& filter);
  void PopulateCookieInfoWithFilter(LocalDataContainer* container,
                                    ScopedBatchUpdateNotifier* notifier,
                                    const base::string16& filter);
  void PopulateDatabaseInfoWithFilter(LocalDataContainer* container,
                                      ScopedBatchUpdateNotifier* notifier,
                                      const base::string16& filter);
  void PopulateLocalStorageInfoWithFilter(LocalDataContainer* container,
                                          ScopedBatchUpdateNotifier* notifier,
                                          const base::string16& filter);
  void PopulateSessionStorageInfoWithFilter(LocalDataContainer* container,
                                            ScopedBatchUpdateNotifier* notifier,
                                            const base::string16& filter);
  void PopulateIndexedDBInfoWithFilter(LocalDataContainer* container,
                                       ScopedBatchUpdateNotifier* notifier,
                                       const base::string16& filter);
  void PopulateFileSystemInfoWithFilter(LocalDataContainer* container,
                                        ScopedBatchUpdateNotifier* notifier,
                                        const base::string16& filter);
  void PopulateQuotaInfoWithFilter(LocalDataContainer* container,
                                   ScopedBatchUpdateNotifier* notifier,
                                   const base::string16& filter);
  void PopulateServerBoundCertInfoWithFilter(
      LocalDataContainer* container,
      ScopedBatchUpdateNotifier* notifier,
      const base::string16& filter);
  void PopulateFlashLSOInfoWithFilter(LocalDataContainer* container,
                                      ScopedBatchUpdateNotifier* notifier,
                                      const base::string16& filter);

  
  
  scoped_ptr<LocalDataContainer> data_container_;

  
  scoped_refptr<ExtensionSpecialStoragePolicy> special_storage_policy_;

  
  
  ObserverList<Observer> cookies_observer_list_;

  
  
  bool group_by_cookie_source_;

  
  
  
  int batch_update_;
};

#endif  
