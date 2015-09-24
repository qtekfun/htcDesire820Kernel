// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_SYNC_ENGINE_SYNCAPI_H_
#define CHROME_BROWSER_SYNC_ENGINE_SYNCAPI_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/tracked.h"
#include "build/build_config.h"
#include "chrome/browser/sync/protocol/password_specifics.pb.h"
#include "chrome/browser/sync/syncable/autofill_migration.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/sync/util/cryptographer.h"
#include "chrome/common/net/gaia/google_service_auth_error.h"
#include "googleurl/src/gurl.h"

class DictionaryValue;
class FilePath;

namespace browser_sync {
class JsBackend;
class ModelSafeWorkerRegistrar;

namespace sessions {
struct SyncSessionSnapshot;
}
}

namespace sync_notifier {
class SyncNotifier;
}  

namespace syncable {
class BaseTransaction;
class DirectoryManager;
class Entry;
class MutableEntry;
class ReadTransaction;
class ScopedDirLookup;
class WriteTransaction;
}

namespace sync_pb {
class AppSpecifics;
class AutofillSpecifics;
class AutofillProfileSpecifics;
class BookmarkSpecifics;
class EntitySpecifics;
class ExtensionSpecifics;
class SessionSpecifics;
class NigoriSpecifics;
class PasswordSpecifics;
class PreferenceSpecifics;
class PasswordSpecifics;
class PasswordSpecificsData;
class ThemeSpecifics;
class TypedUrlSpecifics;
}

namespace sync_api {

class BaseTransaction;
class HttpPostProviderFactory;
class SyncManager;
class WriteTransaction;

struct UserShare {
  UserShare();
  ~UserShare();

  
  
  scoped_ptr<syncable::DirectoryManager> dir_manager;

  
  std::string name;
};

struct SyncCredentials {
  std::string email;
  std::string sync_token;
};

static const int64 kInvalidId = 0;

class BaseNode {
 public:
  
  
  
  virtual bool InitByIdLookup(int64 id) = 0;

  
  
  
  virtual bool InitByClientTagLookup(syncable::ModelType model_type,
      const std::string& tag) = 0;

  
  
  
  
  virtual int64 GetId() const;

  
  int64 GetModificationTime() const;

  
  
  
  int64 GetParentId() const;

  
  
  bool GetIsFolder() const;

  
  
  
  std::wstring GetTitle() const;

  
  
  syncable::ModelType GetModelType() const;

  
  
  const sync_pb::BookmarkSpecifics& GetBookmarkSpecifics() const;

  
  
  
  GURL GetURL() const;

  
  
  
  
  
  
  
  void GetFaviconBytes(std::vector<unsigned char>* output) const;

  
  
  const sync_pb::AppSpecifics& GetAppSpecifics() const;

  
  
  const sync_pb::AutofillSpecifics& GetAutofillSpecifics() const;

  virtual const sync_pb::AutofillProfileSpecifics&
      GetAutofillProfileSpecifics() const;

  
  
  const sync_pb::NigoriSpecifics& GetNigoriSpecifics() const;

  
  
  const sync_pb::PasswordSpecificsData& GetPasswordSpecifics() const;

  
  
  const sync_pb::PreferenceSpecifics& GetPreferenceSpecifics() const;

  
  
  const sync_pb::ThemeSpecifics& GetThemeSpecifics() const;

  
  
  const sync_pb::TypedUrlSpecifics& GetTypedUrlSpecifics() const;

  
  
  const sync_pb::ExtensionSpecifics& GetExtensionSpecifics() const;

  
  
  const sync_pb::SessionSpecifics& GetSessionSpecifics() const;

  
  int64 GetExternalId() const;

  
  
  int64 GetPredecessorId() const;

  
  
  virtual int64 GetSuccessorId() const;

  
  
  virtual int64 GetFirstChildId() const;

  
  virtual const syncable::Entry* GetEntry() const = 0;
  virtual const BaseTransaction* GetTransaction() const = 0;

  
  
  DictionaryValue* ToValue() const;

  
  
  bool ContainsString(const std::string& lowercase_query) const;

 protected:
  BaseNode();
  virtual ~BaseNode();
  
  
  static std::string GenerateSyncableHash(syncable::ModelType model_type,
      const std::string& client_tag);

  
  
  
  
  
  bool DecryptIfNecessary(syncable::Entry* entry);

  
  
  
  
  
  const sync_pb::EntitySpecifics& GetUnencryptedSpecifics(
      const syncable::Entry* entry) const;

 private:
  void* operator new(size_t size);  

  
  sync_pb::EntitySpecifics unencrypted_data_;

  
  scoped_ptr<sync_pb::PasswordSpecificsData> password_data_;

  friend class SyncApiTest;
  FRIEND_TEST_ALL_PREFIXES(SyncApiTest, GenerateSyncableHash);

  DISALLOW_COPY_AND_ASSIGN(BaseNode);
};

class WriteNode : public BaseNode {
 public:
  
  explicit WriteNode(WriteTransaction* transaction);
  virtual ~WriteNode();

  
  

  
  virtual bool InitByIdLookup(int64 id);
  virtual bool InitByClientTagLookup(syncable::ModelType model_type,
      const std::string& tag);

  
  
  
  
  
  
  bool InitByCreation(syncable::ModelType model_type,
                      const BaseNode& parent,
                      const BaseNode* predecessor);

  
  
  
  
  
  
  bool InitUniqueByCreation(syncable::ModelType model_type,
                            const BaseNode& parent,
                            const std::string& client_tag);

  
  
  
  bool InitByTagLookup(const std::string& tag);

  
  void SetIsFolder(bool folder);
  void SetTitle(const std::wstring& title);

  
  
  void SetExternalId(int64 external_id);

  
  void Remove();

  
  
  
  bool SetPosition(const BaseNode& new_parent, const BaseNode* predecessor);

  
  
  void SetBookmarkSpecifics(const sync_pb::BookmarkSpecifics& specifics);

  
  
  
  void SetURL(const GURL& url);
  void SetFaviconBytes(const std::vector<unsigned char>& bytes);

  
  
  void SetAppSpecifics(const sync_pb::AppSpecifics& specifics);

  
  
  void SetAutofillSpecifics(const sync_pb::AutofillSpecifics& specifics);

  void SetAutofillProfileSpecifics(
      const sync_pb::AutofillProfileSpecifics& specifics);

  
  
  void SetNigoriSpecifics(const sync_pb::NigoriSpecifics& specifics);

  
  
  void SetPasswordSpecifics(const sync_pb::PasswordSpecificsData& specifics);

  
  
  void SetPreferenceSpecifics(const sync_pb::PreferenceSpecifics& specifics);

  
  
  void SetThemeSpecifics(const sync_pb::ThemeSpecifics& specifics);

  
  
  void SetTypedUrlSpecifics(const sync_pb::TypedUrlSpecifics& specifics);

  
  
  void SetExtensionSpecifics(const sync_pb::ExtensionSpecifics& specifics);

  
  
  void SetSessionSpecifics(const sync_pb::SessionSpecifics& specifics);

  
  
  void ResetFromSpecifics();

  
  virtual const syncable::Entry* GetEntry() const;

  virtual const BaseTransaction* GetTransaction() const;

 private:
  void* operator new(size_t size);  

  
  void PutModelType(syncable::ModelType model_type);

  
  void PutPredecessor(const BaseNode* predecessor);

  
  
  
  
  
  void PutAppSpecificsAndMarkForSyncing(
      const sync_pb::AppSpecifics& new_value);
  void PutAutofillSpecificsAndMarkForSyncing(
      const sync_pb::AutofillSpecifics& new_value);
  void PutAutofillProfileSpecificsAndMarkForSyncing(
      const sync_pb::AutofillProfileSpecifics& new_value);
  void PutBookmarkSpecificsAndMarkForSyncing(
      const sync_pb::BookmarkSpecifics& new_value);
  void PutNigoriSpecificsAndMarkForSyncing(
      const sync_pb::NigoriSpecifics& new_value);
  void PutPasswordSpecificsAndMarkForSyncing(
      const sync_pb::PasswordSpecifics& new_value);
  void PutPreferenceSpecificsAndMarkForSyncing(
      const sync_pb::PreferenceSpecifics& new_value);
  void PutThemeSpecificsAndMarkForSyncing(
      const sync_pb::ThemeSpecifics& new_value);
  void PutTypedUrlSpecificsAndMarkForSyncing(
      const sync_pb::TypedUrlSpecifics& new_value);
  void PutExtensionSpecificsAndMarkForSyncing(
      const sync_pb::ExtensionSpecifics& new_value);
  void PutSessionSpecificsAndMarkForSyncing(
      const sync_pb::SessionSpecifics& new_value);
  void PutSpecificsAndMarkForSyncing(
      const sync_pb::EntitySpecifics& specifics);

  
  
  void MarkForSyncing();

  
  void EncryptIfNecessary(sync_pb::EntitySpecifics* new_value);

  
  syncable::MutableEntry* entry_;

  
  WriteTransaction* transaction_;

  DISALLOW_COPY_AND_ASSIGN(WriteNode);
};

class ReadNode : public BaseNode {
 public:
  
  
  explicit ReadNode(const BaseTransaction* transaction);
  virtual ~ReadNode();

  
  

  
  virtual bool InitByIdLookup(int64 id);
  virtual bool InitByClientTagLookup(syncable::ModelType model_type,
      const std::string& tag);

  
  
  void InitByRootLookup();

  
  
  
  bool InitByTagLookup(const std::string& tag);

  
  virtual const syncable::Entry* GetEntry() const;
  virtual const BaseTransaction* GetTransaction() const;

 protected:
  ReadNode();

 private:
  void* operator new(size_t size);  

  
  syncable::Entry* entry_;

  
  const BaseTransaction* transaction_;

  DISALLOW_COPY_AND_ASSIGN(ReadNode);
};

class BaseTransaction {
 public:
  
  virtual syncable::BaseTransaction* GetWrappedTrans() const = 0;
  const syncable::ScopedDirLookup& GetLookup() const { return *lookup_; }
  browser_sync::Cryptographer* GetCryptographer() const {
    return cryptographer_;
  }

 protected:
  
  
  
  explicit BaseTransaction(UserShare* share);
  virtual ~BaseTransaction();

  BaseTransaction() { lookup_= NULL; }

 private:
  
  syncable::ScopedDirLookup* lookup_;

  browser_sync::Cryptographer* cryptographer_;

  DISALLOW_COPY_AND_ASSIGN(BaseTransaction);
};

class ReadTransaction : public BaseTransaction {
 public:
  
  explicit ReadTransaction(UserShare* share);

  
  ReadTransaction(UserShare* share, syncable::BaseTransaction* trans);

  virtual ~ReadTransaction();

  
  virtual syncable::BaseTransaction* GetWrappedTrans() const;
 private:
  void* operator new(size_t size);  

  
  syncable::BaseTransaction* transaction_;
  bool close_transaction_;

  DISALLOW_COPY_AND_ASSIGN(ReadTransaction);
};

class WriteTransaction : public BaseTransaction {
 public:
  
  explicit WriteTransaction(UserShare* share);
  virtual ~WriteTransaction();

  
  virtual syncable::BaseTransaction* GetWrappedTrans() const;
  syncable::WriteTransaction* GetWrappedWriteTrans() { return transaction_; }

 protected:
  WriteTransaction() {}

  void SetTransaction(syncable::WriteTransaction* trans) {
      transaction_ = trans;}

 private:
  void* operator new(size_t size);  

  
  syncable::WriteTransaction* transaction_;

  DISALLOW_COPY_AND_ASSIGN(WriteTransaction);
};

class SyncManager {
 public:
  
  
  class SyncInternal;

  
  
  class ExtraPasswordChangeRecordData {
   public:
    ExtraPasswordChangeRecordData();
    explicit ExtraPasswordChangeRecordData(
        const sync_pb::PasswordSpecificsData& data);
    virtual ~ExtraPasswordChangeRecordData();

    
    virtual DictionaryValue* ToValue() const;

    const sync_pb::PasswordSpecificsData& unencrypted() const;
   private:
    sync_pb::PasswordSpecificsData unencrypted_;
  };

  
  
  
  
  struct ChangeRecord {
    enum Action {
      ACTION_ADD,
      ACTION_DELETE,
      ACTION_UPDATE,
    };
    ChangeRecord();
    ~ChangeRecord();

    
    DictionaryValue* ToValue(const BaseTransaction* trans) const;

    int64 id;
    Action action;
    sync_pb::EntitySpecifics specifics;
    linked_ptr<ExtraPasswordChangeRecordData> extra;
  };

  
  struct Status {
    
    
    
    
    enum Summary {
      
      
      INVALID = 0,
      
      
      OFFLINE,
      
      
      OFFLINE_UNSYNCED,
      
      SYNCING,
      
      READY,
      
      CONFLICT,
      
      
      OFFLINE_UNUSABLE,

      SUMMARY_STATUS_COUNT,
    };

    Summary summary;
    bool authenticated;      
    bool server_up;          
                             
    bool server_reachable;   
    bool server_broken;      
                             
    bool notifications_enabled;  

    
    int notifications_received;
    int notifications_sent;

    
    int max_consecutive_errors;

    int unsynced_count;

    int conflicting_count;
    bool syncing;
    
    bool initial_sync_ended;
    
    bool syncer_stuck;

    
    int64 updates_available;
    
    int updates_received;

    
    int tombstone_updates_received;
    bool disk_full;
  };

  
  
  
  
  
  
  
  
  class Observer {
   public:
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void OnChangesApplied(syncable::ModelType model_type,
                                  const BaseTransaction* trans,
                                  const ChangeRecord* changes,
                                  int change_count) = 0;

    
    
    
    
    
    
    
    
    
    
    
    virtual void OnChangesComplete(syncable::ModelType model_type) = 0;

    
    
    virtual void OnSyncCycleCompleted(
        const browser_sync::sessions::SyncSessionSnapshot* snapshot) = 0;

    
    virtual void OnAuthError(const GoogleServiceAuthError& auth_error) = 0;

    
    virtual void OnUpdatedToken(const std::string& token) = 0;

    
    
    
    
    
    virtual void OnPassphraseRequired(bool for_decryption) = 0;

    
    
    
    
    
    virtual void OnPassphraseFailed() = 0;

    
    
    
    
    
    virtual void OnPassphraseAccepted(const std::string& bootstrap_token) = 0;

    
    
    
    
    
    
    
    virtual void OnInitializationComplete() = 0;

    
    
    
    
    virtual void OnStopSyncingPermanently() = 0;

    
    
    virtual void OnClearServerDataSucceeded() = 0;
    virtual void OnClearServerDataFailed() = 0;

    
    virtual void OnEncryptionComplete(
        const syncable::ModelTypeSet& encrypted_types) = 0;

   protected:
    virtual ~Observer();
  };

  typedef Callback0::Type ModeChangeCallback;

  
  SyncManager();
  virtual ~SyncManager();

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool Init(const FilePath& database_location,
            const char* sync_server_and_path,
            int sync_server_port,
            bool use_ssl,
            HttpPostProviderFactory* post_factory,
            browser_sync::ModelSafeWorkerRegistrar* registrar,
            const char* user_agent,
            const SyncCredentials& credentials,
            sync_notifier::SyncNotifier* sync_notifier,
            const std::string& restored_key_for_bootstrapping,
            bool setup_for_test_mode);

  
  
  const std::string& GetAuthenticatedUsername();

  
  
  
  
  bool InitialSyncEndedForAllEnabledTypes();

  syncable::AutofillMigrationState GetAutofillMigrationState();

  void SetAutofillMigrationState(
    syncable::AutofillMigrationState state);

  syncable::AutofillMigrationDebugInfo GetAutofillMigrationDebugInfo();

  void SetAutofillMigrationDebugInfo(
      syncable::AutofillMigrationDebugInfo::PropertyToSet property_to_set,
      const syncable::AutofillMigrationDebugInfo& info);

  
  void MigrateTokens();

  
  void UpdateCredentials(const SyncCredentials& credentials);

  
  void UpdateEnabledTypes();

  
  
  
  void StartSyncing();

  
  
  
  
  
  
  
  
  
  
  
  void SetPassphrase(const std::string& passphrase, bool is_explicit);

  
  
  
  void EncryptDataTypes(const syncable::ModelTypeSet& encrypted_types);

  
  
  
  
  void StartConfigurationMode(ModeChangeCallback* callback);

  
  
  
  void RequestConfig(const syncable::ModelTypeBitSet& types);

  
  
  void RequestNudge(const tracked_objects::Location& nudge_location);

  
  void RequestClearServerData();

  
  
  
  void AddObserver(Observer* observer);

  
  
  
  void RemoveObserver(Observer* observer);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  browser_sync::JsBackend* GetJsBackend();

  
  
  
  Status::Summary GetStatusSummary() const;
  Status GetDetailedStatus() const;

  
  bool IsUsingExplicitPassphrase();

  
  SyncInternal* GetImpl() const;

  
  
  void SaveChanges();

  
  
  void Shutdown();

  UserShare* GetUserShare() const;

  
  
  bool HasUnsyncedItems() const;

  

  void TriggerOnNotificationStateChangeForTest(
      bool notifications_enabled);

  void TriggerOnIncomingNotificationForTest(
      const syncable::ModelTypeBitSet& model_types);

 private:
  
  SyncInternal* data_;

  DISALLOW_COPY_AND_ASSIGN(SyncManager);
};

}  

#endif  
