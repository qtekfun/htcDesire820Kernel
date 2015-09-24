// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_STORE_H_
#define CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_STORE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"

class Profile;

namespace contacts {

class Contact;
typedef std::vector<const Contact*> ContactPointers;
class ContactStoreObserver;

class ContactStore {
 public:
  ContactStore() {}
  virtual ~ContactStore() {}

  
  virtual void Init() = 0;

  
  virtual void AppendContacts(ContactPointers* contacts_out) = 0;

  
  
  virtual const Contact* GetContactById(const std::string& contact_id) = 0;

  
  virtual void AddObserver(ContactStoreObserver* observer) = 0;
  virtual void RemoveObserver(ContactStoreObserver* observer) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ContactStore);
};

class ContactStoreFactory {
 public:
  ContactStoreFactory() {}
  virtual ~ContactStoreFactory() {}

  
  virtual bool CanCreateContactStoreForProfile(Profile* profile) = 0;

  
  
  
  
  
  virtual ContactStore* CreateContactStore(Profile* profile) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ContactStoreFactory);
};

}  

#endif  
