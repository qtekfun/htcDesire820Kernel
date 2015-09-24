// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COOKIES_TREE_MODEL_H_
#define CHROME_BROWSER_COOKIES_TREE_MODEL_H_
#pragma once


#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "base/string16.h"
#include "base/utf_string_conversions.h"
#include "chrome/browser/browsing_data_appcache_helper.h"
#include "chrome/browser/browsing_data_database_helper.h"
#include "chrome/browser/browsing_data_indexed_db_helper.h"
#include "chrome/browser/browsing_data_local_storage_helper.h"
#include "net/base/cookie_monster.h"
#include "ui/base/models/tree_node_model.h"

class CookiesTreeModel;
class CookieTreeAppCacheNode;
class CookieTreeAppCachesNode;
class CookieTreeCookieNode;
class CookieTreeCookiesNode;
class CookieTreeDatabaseNode;
class CookieTreeDatabasesNode;
class CookieTreeLocalStorageNode;
class CookieTreeLocalStoragesNode;
class CookieTreeSessionStorageNode;
class CookieTreeSessionStoragesNode;
class CookieTreeIndexedDBNode;
class CookieTreeIndexedDBsNode;
class CookieTreeOriginNode;

class CookieTreeNode : public ui::TreeNode<CookieTreeNode> {
 public:
  
  
  struct DetailedInfo {
    
    enum NodeType {
      TYPE_ROOT,  
      TYPE_ORIGIN,  
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
    };

    
    
    
    DetailedInfo(const string16& origin, NodeType node_type,
        const net::CookieMonster::CanonicalCookie* cookie,
        const BrowsingDataDatabaseHelper::DatabaseInfo* database_info,
        const BrowsingDataLocalStorageHelper::LocalStorageInfo*
            local_storage_info,
        const BrowsingDataLocalStorageHelper::LocalStorageInfo*
            session_storage_info,
        const appcache::AppCacheInfo* appcache_info,
        const BrowsingDataIndexedDBHelper::IndexedDBInfo* indexed_db_info)
        : origin(UTF16ToWideHack(origin)),
          node_type(node_type),
          cookie(cookie),
          database_info(database_info),
          local_storage_info(local_storage_info),
          session_storage_info(session_storage_info),
          appcache_info(appcache_info),
          indexed_db_info(indexed_db_info) {
      DCHECK((node_type != TYPE_DATABASE) || database_info);
      DCHECK((node_type != TYPE_LOCAL_STORAGE) || local_storage_info);
      DCHECK((node_type != TYPE_SESSION_STORAGE) || session_storage_info);
      DCHECK((node_type != TYPE_APPCACHE) || appcache_info);
      DCHECK((node_type != TYPE_INDEXED_DB) || indexed_db_info);
    }
#if !defined(WCHAR_T_IS_UTF16)
    DetailedInfo(const std::wstring& origin, NodeType node_type,
        const net::CookieMonster::CanonicalCookie* cookie,
        const BrowsingDataDatabaseHelper::DatabaseInfo* database_info,
        const BrowsingDataLocalStorageHelper::LocalStorageInfo*
            local_storage_info,
        const BrowsingDataLocalStorageHelper::LocalStorageInfo*
            session_storage_info,
        const appcache::AppCacheInfo* appcache_info,
        const BrowsingDataIndexedDBHelper::IndexedDBInfo* indexed_db_info)
        : origin(origin),
          node_type(node_type),
          cookie(cookie),
          database_info(database_info),
          local_storage_info(local_storage_info),
          session_storage_info(session_storage_info),
          appcache_info(appcache_info),
          indexed_db_info(indexed_db_info) {
      DCHECK((node_type != TYPE_DATABASE) || database_info);
      DCHECK((node_type != TYPE_LOCAL_STORAGE) || local_storage_info);
      DCHECK((node_type != TYPE_SESSION_STORAGE) || session_storage_info);
      DCHECK((node_type != TYPE_APPCACHE) || appcache_info);
      DCHECK((node_type != TYPE_INDEXED_DB) || indexed_db_info);
    }
#endif

    std::wstring origin;
    NodeType node_type;
    const net::CookieMonster::CanonicalCookie* cookie;
    const BrowsingDataDatabaseHelper::DatabaseInfo* database_info;
    const BrowsingDataLocalStorageHelper::LocalStorageInfo* local_storage_info;
    const BrowsingDataLocalStorageHelper::LocalStorageInfo*
        session_storage_info;
    const appcache::AppCacheInfo* appcache_info;
    const BrowsingDataIndexedDBHelper::IndexedDBInfo* indexed_db_info;
  };

  CookieTreeNode() {}
  explicit CookieTreeNode(const string16& title)
      : ui::TreeNode<CookieTreeNode>(title) {}
  virtual ~CookieTreeNode() {}

  
  
  virtual void DeleteStoredObjects();

  
  virtual CookiesTreeModel* GetModel() const;

  
  
  virtual DetailedInfo GetDetailedInfo() const = 0;

 protected:
  class NodeTitleComparator {
   public:
    bool operator() (const CookieTreeNode* lhs, const CookieTreeNode* rhs);
  };

  void AddChildSortedByTitle(CookieTreeNode* new_child);

 private:

  DISALLOW_COPY_AND_ASSIGN(CookieTreeNode);
};

class CookieTreeRootNode : public CookieTreeNode {
 public:
  explicit CookieTreeRootNode(CookiesTreeModel* model);
  virtual ~CookieTreeRootNode();

  CookieTreeOriginNode* GetOrCreateOriginNode(const GURL& url);

  
  virtual CookiesTreeModel* GetModel() const;
  virtual DetailedInfo GetDetailedInfo() const;

 private:
  CookiesTreeModel* model_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeRootNode);
};

class CookieTreeOriginNode : public CookieTreeNode {
 public:
  
  static std::wstring TitleForUrl(const GURL& url);

  explicit CookieTreeOriginNode(const GURL& url);
  virtual ~CookieTreeOriginNode();

  
  virtual DetailedInfo GetDetailedInfo() const;

  
  CookieTreeCookiesNode* GetOrCreateCookiesNode();
  CookieTreeDatabasesNode* GetOrCreateDatabasesNode();
  CookieTreeLocalStoragesNode* GetOrCreateLocalStoragesNode();
  CookieTreeSessionStoragesNode* GetOrCreateSessionStoragesNode();
  CookieTreeAppCachesNode* GetOrCreateAppCachesNode();
  CookieTreeIndexedDBsNode* GetOrCreateIndexedDBsNode();

  
  
  void CreateContentException(HostContentSettingsMap* content_settings,
                              ContentSetting setting) const;

  
  bool CanCreateContentException() const;

 private:
  
  
  
  
  
  CookieTreeCookiesNode* cookies_child_;
  CookieTreeDatabasesNode* databases_child_;
  CookieTreeLocalStoragesNode* local_storages_child_;
  CookieTreeSessionStoragesNode* session_storages_child_;
  CookieTreeAppCachesNode* appcaches_child_;
  CookieTreeIndexedDBsNode* indexed_dbs_child_;

  
  GURL url_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeOriginNode);
};

class CookieTreeCookieNode : public CookieTreeNode {
 public:
  friend class CookieTreeCookiesNode;

  
  
  explicit CookieTreeCookieNode(net::CookieMonster::CanonicalCookie* cookie);
  virtual ~CookieTreeCookieNode();

  
  virtual void DeleteStoredObjects();
  virtual DetailedInfo GetDetailedInfo() const;

 private:
  
  
  net::CookieMonster::CanonicalCookie* cookie_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeCookieNode);
};

class CookieTreeCookiesNode : public CookieTreeNode {
 public:
  CookieTreeCookiesNode();
  virtual ~CookieTreeCookiesNode();

  virtual DetailedInfo GetDetailedInfo() const;

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
      const appcache::AppCacheInfo* appcache_info);
  virtual ~CookieTreeAppCacheNode() {}

  virtual void DeleteStoredObjects();
  virtual DetailedInfo GetDetailedInfo() const;

 private:
  const appcache::AppCacheInfo* appcache_info_;
  DISALLOW_COPY_AND_ASSIGN(CookieTreeAppCacheNode);
};

class CookieTreeAppCachesNode : public CookieTreeNode {
 public:
  CookieTreeAppCachesNode();
  virtual ~CookieTreeAppCachesNode();

  virtual DetailedInfo GetDetailedInfo() const;

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
      BrowsingDataDatabaseHelper::DatabaseInfo* database_info);
  virtual ~CookieTreeDatabaseNode();

  virtual void DeleteStoredObjects();
  virtual DetailedInfo GetDetailedInfo() const;

 private:
  
  
  BrowsingDataDatabaseHelper::DatabaseInfo* database_info_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeDatabaseNode);
};

class CookieTreeDatabasesNode : public CookieTreeNode {
 public:
  CookieTreeDatabasesNode();
  virtual ~CookieTreeDatabasesNode();

  virtual DetailedInfo GetDetailedInfo() const;

  void AddDatabaseNode(CookieTreeDatabaseNode* child) {
    AddChildSortedByTitle(child);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CookieTreeDatabasesNode);
};


class CookieTreeLocalStorageNode : public CookieTreeNode {
 public:
  
  
  
  explicit CookieTreeLocalStorageNode(
      BrowsingDataLocalStorageHelper::LocalStorageInfo* local_storage_info);
  virtual ~CookieTreeLocalStorageNode();

  
  virtual void DeleteStoredObjects();
  virtual DetailedInfo GetDetailedInfo() const;

 private:
  
  
  BrowsingDataLocalStorageHelper::LocalStorageInfo* local_storage_info_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeLocalStorageNode);
};

class CookieTreeLocalStoragesNode : public CookieTreeNode {
 public:
  CookieTreeLocalStoragesNode();
  virtual ~CookieTreeLocalStoragesNode();

  virtual DetailedInfo GetDetailedInfo() const;

  void AddLocalStorageNode(CookieTreeLocalStorageNode* child) {
    AddChildSortedByTitle(child);
  }

 private:

  DISALLOW_COPY_AND_ASSIGN(CookieTreeLocalStoragesNode);
};


class CookieTreeSessionStorageNode : public CookieTreeNode {
 public:
  
  
  
  explicit CookieTreeSessionStorageNode(
      BrowsingDataLocalStorageHelper::LocalStorageInfo* session_storage_info);
  virtual ~CookieTreeSessionStorageNode();

  
  virtual DetailedInfo GetDetailedInfo() const;

 private:
  
  
  BrowsingDataLocalStorageHelper::LocalStorageInfo* session_storage_info_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeSessionStorageNode);
};

class CookieTreeSessionStoragesNode : public CookieTreeNode {
 public:
  CookieTreeSessionStoragesNode();
  virtual ~CookieTreeSessionStoragesNode();

  virtual DetailedInfo GetDetailedInfo() const;

  void AddSessionStorageNode(CookieTreeSessionStorageNode* child) {
    AddChildSortedByTitle(child);
  }

 private:

  DISALLOW_COPY_AND_ASSIGN(CookieTreeSessionStoragesNode);
};

class CookieTreeIndexedDBNode : public CookieTreeNode {
 public:
  
  
  
  explicit CookieTreeIndexedDBNode(
      BrowsingDataIndexedDBHelper::IndexedDBInfo* indexed_db_info);
  virtual ~CookieTreeIndexedDBNode();

  
  virtual void DeleteStoredObjects();
  virtual DetailedInfo GetDetailedInfo() const;

 private:
  
  
  BrowsingDataIndexedDBHelper::IndexedDBInfo* indexed_db_info_;

  DISALLOW_COPY_AND_ASSIGN(CookieTreeIndexedDBNode);
};

class CookieTreeIndexedDBsNode : public CookieTreeNode {
 public:
  CookieTreeIndexedDBsNode();
  virtual ~CookieTreeIndexedDBsNode();

  virtual DetailedInfo GetDetailedInfo() const;

  void AddIndexedDBNode(CookieTreeIndexedDBNode* child) {
    AddChildSortedByTitle(child);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CookieTreeIndexedDBsNode);
};


class CookiesTreeModel : public ui::TreeNodeModel<CookieTreeNode> {
 public:
  
  
  
  
  
  class Observer : public ui::TreeModelObserver {
   public:
    virtual void TreeModelBeginBatch(CookiesTreeModel* model) {}
    virtual void TreeModelEndBatch(CookiesTreeModel* model) {}
  };

  CookiesTreeModel(
      net::CookieMonster* cookie_monster_,
      BrowsingDataDatabaseHelper* database_helper,
      BrowsingDataLocalStorageHelper* local_storage_helper,
      BrowsingDataLocalStorageHelper* session_storage_helper,
      BrowsingDataAppCacheHelper* appcache_helper,
      BrowsingDataIndexedDBHelper* indexed_db_helper,
      bool use_cookie_source);
  virtual ~CookiesTreeModel();

  
  
  
  virtual void GetIcons(std::vector<SkBitmap>* icons);

  
  
  
  virtual int GetIconIndex(ui::TreeModelNode* node);

  
  void DeleteAllStoredObjects();
  void DeleteCookieNode(CookieTreeNode* cookie_node);

  
  void UpdateSearchResults(const std::wstring& filter);

  
  
  
  
  virtual void AddCookiesTreeObserver(Observer* observer);
  virtual void RemoveCookiesTreeObserver(Observer* observer);

 private:
  enum CookieIconIndex {
    ORIGIN = 0,
    COOKIE = 1,
    DATABASE = 2
  };
  typedef net::CookieList CookieList;
  typedef std::vector<BrowsingDataDatabaseHelper::DatabaseInfo>
      DatabaseInfoList;
  typedef std::vector<BrowsingDataLocalStorageHelper::LocalStorageInfo>
      LocalStorageInfoList;
  typedef std::vector<BrowsingDataLocalStorageHelper::LocalStorageInfo>
      SessionStorageInfoList;
  typedef std::vector<BrowsingDataIndexedDBHelper::IndexedDBInfo>
      IndexedDBInfoList;

  void LoadCookies();
  void LoadCookiesWithFilter(const std::wstring& filter);

  void OnAppCacheModelInfoLoaded();
  void OnDatabaseModelInfoLoaded(const DatabaseInfoList& database_info);
  void OnLocalStorageModelInfoLoaded(
      const LocalStorageInfoList& local_storage_info);
  void OnSessionStorageModelInfoLoaded(
      const LocalStorageInfoList& local_storage_info);
  void OnIndexedDBModelInfoLoaded(
      const IndexedDBInfoList& indexed_db_info);

  void PopulateAppCacheInfoWithFilter(const std::wstring& filter);
  void PopulateDatabaseInfoWithFilter(const std::wstring& filter);
  void PopulateLocalStorageInfoWithFilter(const std::wstring& filter);
  void PopulateSessionStorageInfoWithFilter(const std::wstring& filter);
  void PopulateIndexedDBInfoWithFilter(const std::wstring& filter);

  void NotifyObserverBeginBatch();
  void NotifyObserverEndBatch();

  scoped_refptr<net::CookieMonster> cookie_monster_;
  CookieList all_cookies_;

  scoped_refptr<BrowsingDataAppCacheHelper> appcache_helper_;
  scoped_refptr<BrowsingDataDatabaseHelper> database_helper_;
  scoped_refptr<appcache::AppCacheInfoCollection> appcache_info_;
  DatabaseInfoList database_info_list_;

  scoped_refptr<BrowsingDataLocalStorageHelper> local_storage_helper_;
  scoped_refptr<BrowsingDataLocalStorageHelper> session_storage_helper_;
  scoped_refptr<BrowsingDataIndexedDBHelper> indexed_db_helper_;
  LocalStorageInfoList local_storage_info_list_;
  LocalStorageInfoList session_storage_info_list_;
  IndexedDBInfoList indexed_db_info_list_;

  
  
  ObserverList<Observer> cookies_observer_list_;

  
  
  
  int batch_update_;

  
  
  bool use_cookie_source_;

  friend class CookieTreeAppCacheNode;
  friend class CookieTreeCookieNode;
  friend class CookieTreeDatabaseNode;
  friend class CookieTreeLocalStorageNode;
  friend class CookieTreeIndexedDBNode;

  DISALLOW_COPY_AND_ASSIGN(CookiesTreeModel);
};

#endif  
