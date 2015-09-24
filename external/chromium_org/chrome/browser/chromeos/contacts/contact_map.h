// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_MAP_H_
#define CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_MAP_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/stl_util.h"
#include "base/time/time.h"

namespace contacts {

class Contact;

class ContactMap {
 public:
  typedef std::map<std::string, Contact*> Map;
  typedef Map::const_iterator const_iterator;

  
  enum DeletedContactPolicy {
    
    KEEP_DELETED_CONTACTS,

    
    
    
    DROP_DELETED_CONTACTS,
  };

  ContactMap();
  ~ContactMap();

  bool empty() const { return contacts_.empty(); }
  size_t size() const { return contacts_.size(); }
  const_iterator begin() const { return contacts_.begin(); }
  const_iterator end() const { return contacts_.end(); }

  
  
  const Contact* Find(const std::string& contact_id) const;

  
  void Erase(const std::string& contact_id);

  
  void Clear();

  
  void Merge(scoped_ptr<ScopedVector<Contact> > updated_contacts,
             DeletedContactPolicy policy);

 private:
  Map contacts_;

  
  STLValueDeleter<Map> contacts_deleter_;

  DISALLOW_COPY_AND_ASSIGN(ContactMap);
};

}  

#endif  
