// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEBDATA_AUTOFILL_CHANGE_H__
#define CHROME_BROWSER_WEBDATA_AUTOFILL_CHANGE_H__
#pragma once

#include "chrome/browser/webdata/autofill_entry.h"

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

class AutofillProfileChange : public GenericAutofillChange<std::string> {
 public:
  
  
  
  
  
  AutofillProfileChange(Type type,
                        std::string key,
                        const AutofillProfile* profile);
  virtual ~AutofillProfileChange();

  const AutofillProfile* profile() const { return profile_; }
  bool operator==(const AutofillProfileChange& change) const;

 private:
  
  const AutofillProfile* profile_;
};

class AutofillCreditCardChange : public GenericAutofillChange<std::string> {
 public:
  
  
  
  
  
  AutofillCreditCardChange(Type type,
                           std::string key,
                           const CreditCard* credit_card);
  virtual ~AutofillCreditCardChange();

  const CreditCard* credit_card() const { return credit_card_; }
  bool operator==(const AutofillCreditCardChange& change) const;

 private:
  
  const CreditCard* credit_card_;
};

#endif  
