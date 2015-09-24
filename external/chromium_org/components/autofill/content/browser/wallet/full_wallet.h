// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_FULL_WALLET_H_
#define COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_FULL_WALLET_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "components/autofill/content/browser/wallet/required_action.h"
#include "components/autofill/content/browser/wallet/wallet_address.h"

namespace base {
class DictionaryValue;
}

namespace autofill {

class AutofillType;

namespace wallet {

class FullWalletTest;

class FullWallet {
 public:
  ~FullWallet();

  
  
  static scoped_ptr<FullWallet>
      CreateFullWallet(const base::DictionaryValue& dictionary);

  
  
  static scoped_ptr<FullWallet>
      CreateFullWalletFromClearText(int expiration_month,
                                    int expiration_year,
                                    const std::string& pan,
                                    const std::string& cvn,
                                    scoped_ptr<Address> billing_address,
                                    scoped_ptr<Address> shipping_address);

  
  base::string16 GetInfo(const AutofillType& type);

  
  bool HasRequiredAction(RequiredAction action) const;

  
  
  base::string16 TypeAndLastFourDigits();

  bool operator==(const FullWallet& other) const;
  bool operator!=(const FullWallet& other) const;

  
  const Address* billing_address() const { return billing_address_.get(); }

  
  
  const Address* shipping_address() const { return shipping_address_.get(); }

  const std::vector<RequiredAction>& required_actions() const {
    return required_actions_;
  }
  int expiration_month() const { return expiration_month_; }
  int expiration_year() const { return expiration_year_; }

  void set_one_time_pad(const std::vector<uint8>& one_time_pad) {
    one_time_pad_ = one_time_pad;
  }

 private:
  friend class FullWalletTest;
  friend scoped_ptr<FullWallet> GetTestFullWalletWithRequiredActions(
      const std::vector<RequiredAction>& action);
  friend scoped_ptr<FullWallet> GetTestFullWalletInstrumentOnly();
  FRIEND_TEST_ALL_PREFIXES(FullWalletTest, CreateFullWallet);
  FRIEND_TEST_ALL_PREFIXES(FullWalletTest, CreateFullWalletWithRequiredActions);
  FRIEND_TEST_ALL_PREFIXES(FullWalletTest, RestLengthCorrectDecryptionTest);
  FRIEND_TEST_ALL_PREFIXES(FullWalletTest, RestLengthUnderDecryptionTest);
  FRIEND_TEST_ALL_PREFIXES(FullWalletTest, GetCreditCardInfo);

  FullWallet(int expiration_month,
             int expiration_year,
             const std::string& iin,
             const std::string& encrypted_rest,
             scoped_ptr<Address> billing_address,
             scoped_ptr<Address> shipping_address,
             const std::vector<RequiredAction>& required_actions);

  
  void DecryptCardInfo();

  
  
  const std::string& GetPan();

  
  
  const std::string& GetCvn();

  
  int expiration_month_;

  
  int expiration_year_;

  
  std::string pan_;

  
  std::string cvn_;

  
  std::string iin_;

  
  std::string encrypted_rest_;

  
  scoped_ptr<Address> billing_address_;

  
  scoped_ptr<Address> shipping_address_;

  
  
  std::vector<RequiredAction> required_actions_;

  
  std::vector<uint8> one_time_pad_;

  DISALLOW_COPY_AND_ASSIGN(FullWallet);
};

}  
}  

#endif  
