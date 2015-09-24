// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_MANAGER_STUB_H_
#define CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_MANAGER_STUB_H_

#include "chrome/browser/chromeos/contacts/contact_manager.h"

#include "base/memory/scoped_vector.h"

namespace contacts {

class ContactManagerStub : public ContactManagerInterface {
 public:
  explicit ContactManagerStub(Profile* profile);
  virtual ~ContactManagerStub();

  
  void SetContacts(const ContactPointers& contacts);

  
  void NotifyObserversAboutUpdatedContacts();

  
  virtual base::WeakPtr<ContactManagerInterface> GetWeakPtr() OVERRIDE;
  virtual void AddObserver(ContactManagerObserver* observer, Profile* profile)
      OVERRIDE;
  virtual void RemoveObserver(ContactManagerObserver* observer,
                              Profile* profile) OVERRIDE;
  virtual scoped_ptr<ContactPointers> GetAllContacts(Profile* profile) OVERRIDE;
  virtual const Contact* GetContactById(Profile* profile,
                                        const std::string& contact_id) OVERRIDE;

 private:
  
  
  Profile* profile_;  

  
  ObserverList<ContactManagerObserver> observers_;

  
  ScopedVector<Contact> contacts_;

  
  
  base::WeakPtrFactory<ContactManagerInterface> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ContactManagerStub);
};

}  

#endif  
