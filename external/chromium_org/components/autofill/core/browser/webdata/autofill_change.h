// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_CHANGE_H__
#define COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_CHANGE_H__

#include <vector>

#include "components/autofill/core/browser/webdata/autofill_entry.h"

namespace autofill {

class AutofillProfile;
class CreditCard;

template <typename KeyType>
class GenericAutofillChange {
 public:
  typedef enum {
    ADD,
    UPDATE,
    REMOVE
  } Type;

  virtual ~GenericAutofillChange() {}

  Type type() const { return type_; }
  const KeyType& key() const { return key_; }

 protected:
  GenericAutofillChange(Type type, const KeyType& key)
      : type_(type),
        key_(key) {}
 private:
  Type type_;
  KeyType key_;
};

class AutofillChange : public GenericAutofillChange<AutofillKey> {
 public:
  AutofillChange(Type type, const AutofillKey& key);
  virtual ~AutofillChange();
  bool operator==(const AutofillChange& change) const {
    return type() == change.type() && key() == change.key();
  }
};

typedef std::vector<AutofillChange> AutofillChangeList;

class AutofillProfileChange : public GenericAutofillChange<std::string> {
 public:
  
  
  
  
  
  AutofillProfileChange(Type type,
                        const std::string& key,
                        const AutofillProfile* profile);
  virtual ~AutofillProfileChange();

  const AutofillProfile* profile() const { return profile_; }
  bool operator==(const AutofillProfileChange& change) const;

 private:
  
  const AutofillProfile* profile_;
};

}  

#endif  
