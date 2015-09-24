// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_IMPORTER_NSS_DECRYPTOR_SYSTEM_NSS_H_
#define CHROME_UTILITY_IMPORTER_NSS_DECRYPTOR_SYSTEM_NSS_H_

#include <secmodt.h>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace autofill {
struct PasswordForm;
}

namespace base {
class FilePath;
}

class NSSDecryptor {
 public:
  NSSDecryptor();
  ~NSSDecryptor();

  
  bool Init(const base::FilePath& dll_path, const base::FilePath& db_path);

  
  
  string16 Decrypt(const std::string& crypt) const;

  
  
  
  void ParseSignons(const std::string& content,
                    std::vector<autofill::PasswordForm>* forms);

  
  
  
  bool ReadAndParseSignons(const base::FilePath& sqlite_file,
                           std::vector<autofill::PasswordForm>* forms);
 private:
  
  
  void FreeSlot(PK11SlotInfo* slot) const {}
  PK11SlotInfo* GetKeySlotForDB() const { return db_slot_; }

  SECStatus PK11SDR_DecryptWithSlot(
      PK11SlotInfo* slot, SECItem* data, SECItem* result, void* cx) const;

  bool is_nss_initialized_;
  PK11SlotInfo* db_slot_;

  DISALLOW_COPY_AND_ASSIGN(NSSDecryptor);
};

#endif  
