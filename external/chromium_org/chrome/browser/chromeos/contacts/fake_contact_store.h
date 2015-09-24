// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CONTACTS_FAKE_CONTACT_STORE_H_
#define CHROME_BROWSER_CHROMEOS_CONTACTS_FAKE_CONTACT_STORE_H_

#include "chrome/browser/chromeos/contacts/contact_store.h"

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "base/stl_util.h"

class Profile;

namespace contacts {

class Contact;
class FakeContactStoreFactory;
typedef std::vector<const Contact*> ContactPointers;

class FakeContactStore : public ContactStore {
 public:
  explicit FakeContactStore(FakeContactStoreFactory* factory);
  virtual ~FakeContactStore();

  
  
  void SetContacts(const ContactPointers& contacts);

  
  void NotifyObserversAboutContactsUpdate();

  
  virtual void Init() OVERRIDE;
  virtual void AppendContacts(ContactPointers* contacts_out) OVERRIDE;
  virtual const Contact* GetContactById(const std::string& contact_id) OVERRIDE;
  virtual void AddObserver(ContactStoreObserver* observer) OVERRIDE;
  virtual void RemoveObserver(ContactStoreObserver* observer) OVERRIDE;

 private:
  
  typedef std::map<std::string, Contact*> ContactMap;

  
  FakeContactStoreFactory* factory_;

  ObserverList<ContactStoreObserver> observers_;

  
  ContactMap contacts_;

  
  STLValueDeleter<ContactMap> contacts_deleter_;

  DISALLOW_COPY_AND_ASSIGN(FakeContactStore);
};

class FakeContactStoreFactory : public ContactStoreFactory {
 public:
  FakeContactStoreFactory();
  virtual ~FakeContactStoreFactory();

  void set_permit_store_creation(bool permit) {
    permit_store_creation_ = permit;
  }

  
  
  FakeContactStore* GetContactStoreForProfile(Profile* profile);

  
  
  void RemoveStore(FakeContactStore* store);

  
  virtual bool CanCreateContactStoreForProfile(Profile* profile) OVERRIDE;
  virtual ContactStore* CreateContactStore(Profile* profile) OVERRIDE;

 private:
  typedef std::map<Profile*, FakeContactStore*> ProfileStoreMap;

  
  
  
  ProfileStoreMap stores_;

  
  bool permit_store_creation_;

  DISALLOW_COPY_AND_ASSIGN(FakeContactStoreFactory);
};

}  

#endif  
