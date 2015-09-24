// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SSL_DEFAULT_SERVER_BOUND_CERT_STORE_H_
#define NET_SSL_DEFAULT_SERVER_BOUND_CERT_STORE_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "net/base/net_export.h"
#include "net/ssl/server_bound_cert_store.h"

namespace net {

class NET_EXPORT DefaultServerBoundCertStore : public ServerBoundCertStore {
 public:
  class PersistentStore;

  
  
  typedef std::map<std::string, ServerBoundCert*> ServerBoundCertMap;

  
  
  
  
  
  explicit DefaultServerBoundCertStore(PersistentStore* store);

  virtual ~DefaultServerBoundCertStore();

  
  virtual int GetServerBoundCert(
      const std::string& server_identifier,
      base::Time* expiration_time,
      std::string* private_key_result,
      std::string* cert_result,
      const GetCertCallback& callback) OVERRIDE;
  virtual void SetServerBoundCert(
      const std::string& server_identifier,
      base::Time creation_time,
      base::Time expiration_time,
      const std::string& private_key,
      const std::string& cert) OVERRIDE;
  virtual void DeleteServerBoundCert(
      const std::string& server_identifier,
      const base::Closure& callback) OVERRIDE;
  virtual void DeleteAllCreatedBetween(
      base::Time delete_begin,
      base::Time delete_end,
      const base::Closure& callback) OVERRIDE;
  virtual void DeleteAll(const base::Closure& callback) OVERRIDE;
  virtual void GetAllServerBoundCerts(
      const GetCertListCallback& callback) OVERRIDE;
  virtual int GetCertCount() OVERRIDE;
  virtual void SetForceKeepSessionState() OVERRIDE;

 private:
  class Task;
  class GetServerBoundCertTask;
  class SetServerBoundCertTask;
  class DeleteServerBoundCertTask;
  class DeleteAllCreatedBetweenTask;
  class GetAllServerBoundCertsTask;

  static const size_t kMaxCerts;

  
  void DeleteAllInMemory();

  
  
  
  
  
  void InitIfNecessary() {
    if (!initialized_) {
      if (store_.get()) {
        InitStore();
      } else {
        loaded_ = true;
      }
      initialized_ = true;
    }
  }

  
  
  void InitStore();

  
  void OnLoaded(scoped_ptr<ScopedVector<ServerBoundCert> > certs);

  
  
  void SyncSetServerBoundCert(
      const std::string& server_identifier,
      base::Time creation_time,
      base::Time expiration_time,
      const std::string& private_key,
      const std::string& cert);
  void SyncDeleteServerBoundCert(const std::string& server_identifier);
  void SyncDeleteAllCreatedBetween(base::Time delete_begin,
                                   base::Time delete_end);
  void SyncGetAllServerBoundCerts(ServerBoundCertList* cert_list);

  
  void EnqueueTask(scoped_ptr<Task> task);
  
  
  void RunOrEnqueueTask(scoped_ptr<Task> task);

  
  
  void InternalDeleteServerBoundCert(const std::string& server);

  
  
  
  void InternalInsertServerBoundCert(const std::string& server_identifier,
                                     ServerBoundCert* cert);

  
  
  bool initialized_;

  
  
  bool loaded_;

  
  ScopedVector<Task> waiting_tasks_;
  base::TimeTicks waiting_tasks_start_time_;

  scoped_refptr<PersistentStore> store_;

  ServerBoundCertMap server_bound_certs_;

  base::WeakPtrFactory<DefaultServerBoundCertStore> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DefaultServerBoundCertStore);
};

typedef base::RefCountedThreadSafe<DefaultServerBoundCertStore::PersistentStore>
    RefcountedPersistentStore;

class NET_EXPORT DefaultServerBoundCertStore::PersistentStore
    : public RefcountedPersistentStore {
 public:
  typedef base::Callback<void(scoped_ptr<ScopedVector<ServerBoundCert> >)>
      LoadedCallback;

  
  
  
  
  virtual void Load(const LoadedCallback& loaded_callback) = 0;

  virtual void AddServerBoundCert(const ServerBoundCert& cert) = 0;

  virtual void DeleteServerBoundCert(const ServerBoundCert& cert) = 0;

  
  
  virtual void SetForceKeepSessionState() = 0;

 protected:
  friend class base::RefCountedThreadSafe<PersistentStore>;

  PersistentStore();
  virtual ~PersistentStore();

 private:
  DISALLOW_COPY_AND_ASSIGN(PersistentStore);
};

}  

#endif  
