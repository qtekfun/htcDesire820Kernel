// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_TEST_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_TEST_UTIL_H_

#include <string>

#include "base/memory/scoped_vector.h"
#include "chrome/browser/chromeos/contacts/contact.pb.h"
#include "ui/gfx/size.h"

namespace contacts {

typedef std::vector<const Contact*> ContactPointers;
class ContactMap;

namespace test {

std::string ContactToString(const Contact& contact);

std::string ContactsToString(const ContactPointers& contacts);
std::string ContactsToString(const ScopedVector<Contact>& contacts);

std::string VarContactsToString(int num_contacts, ...);

std::string ContactMapToString(const ContactMap& contact_map);

void CopyContacts(const ContactPointers& source,
                  ScopedVector<Contact>* dest);
void CopyContacts(const ScopedVector<Contact>& source,
                  ScopedVector<Contact>* dest);

void InitContact(const std::string& contact_id,
                 const std::string& name_suffix,
                 bool deleted,
                 Contact* contact);

void AddEmailAddress(const std::string& address,
                     Contact_AddressType_Relation relation,
                     const std::string& label,
                     bool primary,
                     Contact* contact);

void AddPhoneNumber(const std::string& number,
                    Contact_AddressType_Relation relation,
                    const std::string& label,
                    bool primary,
                    Contact* contact);

void AddPostalAddress(const std::string& address,
                      Contact_AddressType_Relation relation,
                      const std::string& label,
                      bool primary,
                      Contact* contact);

void AddInstantMessagingAddress(
    const std::string& address,
    Contact_InstantMessagingAddress_Protocol protocol,
    Contact_AddressType_Relation relation,
    const std::string& label,
    bool primary,
    Contact* contact);

void SetPhoto(const gfx::Size& size, Contact* contact);

}  
}  

#endif  
