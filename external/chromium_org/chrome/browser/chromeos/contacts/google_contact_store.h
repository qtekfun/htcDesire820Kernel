// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CONTACTS_GOOGLE_CONTACT_STORE_H_
#define CHROME_BROWSER_CHROMEOS_CONTACTS_GOOGLE_CONTACT_STORE_H_

#include "chrome/browser/chromeos/contacts/contact_store.h"

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/chromeos/contacts/contact_map.h"
#include "net/base/network_change_notifier.h"

class Profile;

namespace net {
class URLRequestContextGetter;
}  

namespace contacts {

class Contact;
class ContactDatabaseInterface;
class GDataContactsServiceInterface;
class UpdateMetadata;

class GoogleContactStore
    : public ContactStore,
      public net::NetworkChangeNotifier::ConnectionTypeObserver {
 public:
  class TestAPI {
   public:
    explicit TestAPI(GoogleContactStore* store);
    ~TestAPI();

    bool update_scheduled() { return store_->update_timer_.IsRunning(); }
    base::Time last_contact_update_time() const {
      return store_->last_contact_update_time_;
    }
    void set_current_time(const base::Time& time) {
      store_->current_time_for_testing_ = time;
    }

    
    void SetDatabase(ContactDatabaseInterface* db);

    
    
    void SetGDataService(GDataContactsServiceInterface* service);

    
    void DoUpdate();

    
    void NotifyAboutNetworkStateChange(bool online);

    
    
    scoped_ptr<ContactPointers> GetLoadedContacts();

   private:
    GoogleContactStore* store_;  

    DISALLOW_COPY_AND_ASSIGN(TestAPI);
  };

  GoogleContactStore(
      net::URLRequestContextGetter* url_request_context_getter,
      Profile* profile);
  virtual ~GoogleContactStore();

  
  virtual void Init() OVERRIDE;
  virtual void AppendContacts(ContactPointers* contacts_out) OVERRIDE;
  virtual const Contact* GetContactById(const std::string& contact_id) OVERRIDE;
  virtual void AddObserver(ContactStoreObserver* observer) OVERRIDE;
  virtual void RemoveObserver(ContactStoreObserver* observer) OVERRIDE;

  
  virtual void OnConnectionTypeChanged(
      net::NetworkChangeNotifier::ConnectionType type) OVERRIDE;

 private:
  
  
  base::Time GetCurrentTime() const;

  
  
  void DestroyDatabase();

  
  void UpdateContacts();

  
  void ScheduleUpdate(bool last_update_was_successful);

  
  
  void MergeContacts(bool is_full_update,
                     scoped_ptr<ScopedVector<Contact> > updated_contacts);

  
  
  void OnDownloadSuccess(bool is_full_update,
                         const base::Time& update_start_time,
                         scoped_ptr<ScopedVector<Contact> > updated_contacts);

  
  void OnDownloadFailure();

  
  
  
  void OnDatabaseInitialized(bool success);

  
  
  void OnDatabaseContactsLoaded(bool success,
                                scoped_ptr<ScopedVector<Contact> > contacts,
                                scoped_ptr<UpdateMetadata> metadata);

  
  
  void OnDatabaseContactsSaved(bool success);

  net::URLRequestContextGetter* url_request_context_getter_;  

  Profile* profile_;  

  ObserverList<ContactStoreObserver> observers_;

  
  ContactMap contacts_;

  
  
  base::Time last_contact_update_time_;

  
  scoped_ptr<GDataContactsServiceInterface> gdata_service_;

  
  ContactDatabaseInterface* db_;

  
  base::OneShotTimer<GoogleContactStore> update_timer_;

  
  base::Time last_successful_update_start_time_;

  
  
  base::TimeDelta update_delay_on_next_failure_;

  
  
  bool is_online_;

  
  
  bool should_update_when_online_;

  
  
  base::Time current_time_for_testing_;

  
  
  base::WeakPtrFactory<GoogleContactStore> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(GoogleContactStore);
};

class GoogleContactStoreFactory : public ContactStoreFactory {
 public:
  GoogleContactStoreFactory();
  virtual ~GoogleContactStoreFactory();

  
  virtual bool CanCreateContactStoreForProfile(Profile* profile) OVERRIDE;
  virtual ContactStore* CreateContactStore(Profile* profile) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(GoogleContactStoreFactory);
};

}  

#endif  
