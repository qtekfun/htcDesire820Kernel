// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_MANAGER_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/stl_util.h"
#include "chrome/browser/chromeos/contacts/contact_store_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

namespace contacts {

class Contact;
typedef std::vector<const Contact*> ContactPointers;

class ContactManagerObserver;
class ContactStore;
class ContactStoreFactory;

class ContactManagerInterface {
 public:
  ContactManagerInterface() {}
  virtual ~ContactManagerInterface() {}

  
  virtual base::WeakPtr<ContactManagerInterface> GetWeakPtr() = 0;

  
  virtual void AddObserver(ContactManagerObserver* observer,
                           Profile* profile) = 0;
  virtual void RemoveObserver(ContactManagerObserver* observer,
                              Profile* profile) = 0;

  
  
  
  virtual scoped_ptr<ContactPointers> GetAllContacts(Profile* profile) = 0;

  
  
  virtual const Contact* GetContactById(Profile* profile,
                                        const std::string& contact_id) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ContactManagerInterface);
};

class ContactManager : public ContactManagerInterface,
                       public ContactStoreObserver,
                       public content::NotificationObserver {
 public:
  static ContactManager* GetInstance();

  ContactManager();
  virtual ~ContactManager();

  
  
  void SetContactStoreForTesting(scoped_ptr<ContactStoreFactory> factory);

  void Init();

  
  virtual base::WeakPtr<ContactManagerInterface> GetWeakPtr() OVERRIDE;
  virtual void AddObserver(ContactManagerObserver* observer,
                           Profile* profile) OVERRIDE;
  virtual void RemoveObserver(ContactManagerObserver* observer,
                              Profile* profile) OVERRIDE;
  virtual scoped_ptr<ContactPointers> GetAllContacts(Profile* profile) OVERRIDE;
  virtual const Contact* GetContactById(Profile* profile,
                                        const std::string& contact_id) OVERRIDE;

  
  virtual void OnContactsUpdated(ContactStore* store) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  typedef ObserverList<ContactManagerObserver> Observers;
  typedef std::map<Profile*, ContactStore*> ContactStoreMap;
  typedef std::map<Profile*, Observers*> ProfileObserversMap;

  
  
  Observers* GetObserversForProfile(Profile* profile, bool create);

  
  void HandleProfileCreated(Profile* profile);
  void HandleProfileDestroyed(Profile* profile);

  content::NotificationRegistrar registrar_;

  
  
  ProfileObserversMap profile_observers_;

  
  STLValueDeleter<ProfileObserversMap> profile_observers_deleter_;

  
  scoped_ptr<ContactStoreFactory> contact_store_factory_;

  
  ContactStoreMap contact_stores_;

  
  STLValueDeleter<ContactStoreMap> contact_stores_deleter_;

  
  
  base::WeakPtrFactory<ContactManagerInterface> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ContactManager);
};

}  

#endif  
