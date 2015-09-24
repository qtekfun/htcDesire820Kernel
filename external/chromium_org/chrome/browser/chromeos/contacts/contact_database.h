// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_DATABASE_H_
#define CHROME_BROWSER_CHROMEOS_CONTACTS_CONTACT_DATABASE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"

namespace base {
class SequencedTaskRunner;
}

namespace leveldb {
class DB;
}

namespace contacts {

class Contact;
class UpdateMetadata;
typedef std::vector<const Contact*> ContactPointers;

class ContactDatabaseInterface {
 public:
  typedef std::vector<std::string> ContactIds;
  typedef base::Callback<void(bool success)> InitCallback;
  typedef base::Callback<void(bool success)> SaveCallback;
  typedef base::Callback<void(bool success,
                              scoped_ptr<ScopedVector<Contact> >,
                              scoped_ptr<UpdateMetadata>)>
      LoadCallback;

  
  
  virtual void DestroyOnUIThread() {}

  
  
  virtual void Init(const base::FilePath& database_dir,
                    InitCallback callback) = 0;

  
  
  
  
  
  
  virtual void SaveContacts(scoped_ptr<ContactPointers> contacts_to_save,
                            scoped_ptr<ContactIds> contact_ids_to_delete,
                            scoped_ptr<UpdateMetadata> metadata,
                            bool is_full_update,
                            SaveCallback callback) = 0;

  
  
  virtual void LoadContacts(LoadCallback callback) = 0;

 protected:
  virtual ~ContactDatabaseInterface() {}
};

class ContactDatabase : public ContactDatabaseInterface {
 public:
  ContactDatabase();

  
  virtual void DestroyOnUIThread() OVERRIDE;
  virtual void Init(const base::FilePath& database_dir,
                    InitCallback callback) OVERRIDE;
  virtual void SaveContacts(scoped_ptr<ContactPointers> contacts_to_save,
                            scoped_ptr<ContactIds> contact_ids_to_delete,
                            scoped_ptr<UpdateMetadata> metadata,
                            bool is_full_update,
                            SaveCallback callback) OVERRIDE;
  virtual void LoadContacts(LoadCallback callback) OVERRIDE;

 protected:
  virtual ~ContactDatabase();

 private:
  
  bool IsRunByTaskRunner() const;

  
  void DestroyFromTaskRunner();

  
  
  
  
  void RunInitCallback(InitCallback callback, const bool* success);
  void RunSaveCallback(SaveCallback callback, const bool* success);
  void RunLoadCallback(LoadCallback callback,
                       const bool* success,
                       scoped_ptr<ScopedVector<Contact> > contacts,
                       scoped_ptr<UpdateMetadata> metadata);

  
  void InitFromTaskRunner(const base::FilePath& database_dir, bool* success);

  
  void SaveContactsFromTaskRunner(scoped_ptr<ContactPointers> contacts_to_save,
                                  scoped_ptr<ContactIds> contact_ids_to_delete,
                                  scoped_ptr<UpdateMetadata> metadata,
                                  bool is_full_update,
                                  bool* success);

  
  void LoadContactsFromTaskRunner(bool* success,
                                  ScopedVector<Contact>* contacts,
                                  UpdateMetadata* metadata);

  
  scoped_refptr<base::SequencedTaskRunner> task_runner_;

  scoped_ptr<leveldb::DB> db_;

  
  
  base::WeakPtrFactory<ContactDatabase> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ContactDatabase);
};

}  

#endif  
