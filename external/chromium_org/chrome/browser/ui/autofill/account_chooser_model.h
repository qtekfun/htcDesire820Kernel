// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_ACCOUNT_CHOOSER_MODEL_H_
#define CHROME_BROWSER_UI_AUTOFILL_ACCOUNT_CHOOSER_MODEL_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "components/autofill/core/browser/autofill_manager_delegate.h"
#include "ui/base/models/simple_menu_model.h"

class AutofillMetrics;
class Profile;

namespace autofill {

class AccountChooserModel;

class AccountChooserModelDelegate {
 public:
  virtual ~AccountChooserModelDelegate();

  
  virtual void AccountChooserWillShow() = 0;

  
  virtual void AccountChoiceChanged() = 0;

  
  virtual void AddAccount() = 0;

  
  virtual void UpdateAccountChooserView() = 0;
};

class AccountChooserModel : public ui::SimpleMenuModel,
                            public ui::SimpleMenuModel::Delegate {
 public:
  
  
  AccountChooserModel(AccountChooserModelDelegate* delegate,
                      Profile* profile,
                      bool disable_wallet,
                      const AutofillMetrics& metric_logger);
  virtual ~AccountChooserModel();

  
  virtual void MenuWillShow() OVERRIDE;

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;
  virtual void MenuWillShow(ui::SimpleMenuModel* source) OVERRIDE;

  
  void SelectWalletAccount(size_t user_index);

  
  void SelectUseAutofill();

  
  bool HasAccountsToChoose() const;

  
  
  void SetWalletAccounts(const std::vector<std::string>& accounts,
                         size_t active_index);

  
  
  void ClearWalletAccounts();

  
  
  
  base::string16 GetActiveWalletAccountName() const;

  
  size_t GetActiveWalletAccountIndex() const;

  
  
  void SetHadWalletError();

  
  
  void SetHadWalletSigninError();

  
  bool WalletIsSelected() const;

 protected:
  
  
  static const int kWalletAddAccountId;
  
  static const int kAutofillItemId;
  
  static const int kWalletAccountsStartId;

 private:
  
  void ReconstructMenuItems();

  AccountChooserModelDelegate* delegate_;

  
  int checked_item_;

  
  bool had_wallet_error_;

  
  const AutofillMetrics& metric_logger_;

  
  
  std::vector<std::string> wallet_accounts_;

  DISALLOW_COPY_AND_ASSIGN(AccountChooserModel);
};

}  

#endif  
