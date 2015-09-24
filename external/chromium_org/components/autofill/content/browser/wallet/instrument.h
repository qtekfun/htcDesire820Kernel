// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_INSTRUMENT_H_
#define COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_INSTRUMENT_H_

#include <string>
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"

namespace base {
class DictionaryValue;
}

namespace autofill {

class AutofillProfile;
class CreditCard;

namespace wallet {

class Address;

class Instrument {
 public:
  enum FormOfPayment {
    UNKNOWN,
    VISA,
    MASTER_CARD,
    AMEX,
    DISCOVER,
    JCB,
  };

  
  Instrument(const CreditCard& card,
             const base::string16& card_verification_number,
             const AutofillProfile& profile);

  Instrument(const base::string16& primary_account_number,
             const base::string16& card_verification_number,
             int expiration_month,
             int expiration_year,
             FormOfPayment form_of_payment,
             scoped_ptr<Address> address);

  Instrument(const Instrument& instrument);

  ~Instrument();

  scoped_ptr<base::DictionaryValue> ToDictionary() const;

  const base::string16& primary_account_number() const {
    return primary_account_number_;
  }
  const base::string16& card_verification_number() const {
    return card_verification_number_;
  }
  int expiration_month() const { return expiration_month_; }
  int expiration_year() const { return expiration_year_; }
  const Address* address() const { return address_.get(); }
  FormOfPayment form_of_payment() const { return form_of_payment_; }
  const base::string16& last_four_digits() const { return last_four_digits_; }
  const std::string& object_id() const { return object_id_; }

 private:
  void Init();

  
  base::string16 primary_account_number_;

  
  base::string16 card_verification_number_;

  
  int expiration_month_;

  
  int expiration_year_;

  
  FormOfPayment form_of_payment_;

  
  scoped_ptr<Address> address_;

  
  base::string16 last_four_digits_;

  
  std::string object_id_;

  DISALLOW_ASSIGN(Instrument);
};

}  
}  

#endif  
