// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_REQUIRED_ACTION_H_
#define COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_REQUIRED_ACTION_H_

#include <string>

namespace autofill {
namespace wallet {

enum RequiredAction {
  UNKNOWN_TYPE = 0,  
  CHOOSE_ANOTHER_INSTRUMENT_OR_ADDRESS,
  SETUP_WALLET,
  ACCEPT_TOS,
  GAIA_AUTH,
  UPDATE_EXPIRATION_DATE,
  UPGRADE_MIN_ADDRESS,
  INVALID_FORM_FIELD,
  VERIFY_CVV,
  PASSIVE_GAIA_AUTH,
  REQUIRE_PHONE_NUMBER,
};

bool ActionAppliesToFullWallet(RequiredAction action);
bool ActionAppliesToSaveToWallet(RequiredAction action);
bool ActionAppliesToWalletItems(RequiredAction action);

RequiredAction ParseRequiredActionFromString(const std::string& str);

}  
}  

#endif  
