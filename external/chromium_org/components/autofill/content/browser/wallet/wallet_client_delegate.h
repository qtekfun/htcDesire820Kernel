// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_BROWSER_WALLET_WALLET_CLIENT_OBSERVER_H_
#define COMPONENTS_AUTOFILL_BROWSER_WALLET_WALLET_CLIENT_OBSERVER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "components/autofill/content/browser/wallet/form_field_error.h"
#include "components/autofill/content/browser/wallet/wallet_client.h"
#include "components/autofill/core/browser/autofill_manager_delegate.h"

class AutofillMetrics;

namespace autofill {
namespace wallet {

class FullWallet;
class WalletItems;

class WalletClientDelegate {
 public:
  
  
  

  
  
  virtual const AutofillMetrics& GetMetricLogger() const = 0;

  
  virtual std::string GetRiskData() const = 0;

  
  
  virtual std::string GetWalletCookieValue() const = 0;

  
  virtual bool IsShippingAddressRequired() const = 0;

  
  
  

  
  virtual void OnDidAcceptLegalDocuments() = 0;

  
  virtual void OnDidAuthenticateInstrument(bool success) = 0;

  
  
  virtual void OnDidGetFullWallet(scoped_ptr<FullWallet> full_wallet) = 0;

  
  
  virtual void OnDidGetWalletItems(scoped_ptr<WalletItems> wallet_items) = 0;

  
  
  
  
  
  virtual void OnDidSaveToWallet(
      const std::string& instrument_id,
      const std::string& address_id,
      const std::vector<RequiredAction>& required_actions,
      const std::vector<FormFieldError>& form_field_errors) = 0;

  
  virtual void OnWalletError(WalletClient::ErrorType error_type) = 0;

 protected:
  virtual ~WalletClientDelegate() {}
};

}  
}  

#endif  
