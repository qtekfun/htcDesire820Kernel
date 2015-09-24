// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_DATA_MODEL_WRAPPER_H_
#define CHROME_BROWSER_UI_AUTOFILL_DATA_MODEL_WRAPPER_H_

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/autofill/autofill_dialog_types.h"
#include "components/autofill/content/browser/wallet/wallet_items.h"
#include "components/autofill/core/browser/field_types.h"

namespace gfx {
class Image;
}

namespace autofill {

class AutofillDataModel;
class AutofillProfile;
class AutofillType;
class CreditCard;
class FormStructure;

namespace wallet {
class Address;
class FullWallet;
}

class DataModelWrapper {
 public:
  virtual ~DataModelWrapper();

  
  
  void FillInputs(DetailInputs* inputs);

  
  
  virtual base::string16 GetInfo(const AutofillType& type) const = 0;

  
  
  virtual base::string16 GetInfoForDisplay(const AutofillType& type) const;

  
  virtual gfx::Image GetIcon();

  
  
  
  
  
  
  
  virtual bool GetDisplayText(base::string16* vertically_compact,
                              base::string16* horizontally_compact);

  
  
  
  
  bool FillFormStructure(
      const DetailInputs& inputs,
      const InputFieldComparator& compare,
      FormStructure* form_structure) const;

 protected:
  DataModelWrapper();

 private:
  
  
  base::string16 GetAddressDisplayText(const base::string16& separator);

  DISALLOW_COPY_AND_ASSIGN(DataModelWrapper);
};

class EmptyDataModelWrapper : public DataModelWrapper {
 public:
  EmptyDataModelWrapper();
  virtual ~EmptyDataModelWrapper();

  virtual base::string16 GetInfo(const AutofillType& type) const OVERRIDE;

 protected:
  DISALLOW_COPY_AND_ASSIGN(EmptyDataModelWrapper);
};

class AutofillProfileWrapper : public DataModelWrapper {
 public:
  explicit AutofillProfileWrapper(const AutofillProfile* profile);
  AutofillProfileWrapper(const AutofillProfile* profile,
                         const AutofillType& variant_type,
                         size_t variant);
  virtual ~AutofillProfileWrapper();

  virtual base::string16 GetInfo(const AutofillType& type) const OVERRIDE;
  virtual base::string16 GetInfoForDisplay(const AutofillType& type) const
      OVERRIDE;

 protected:
  
  
  size_t GetVariantForType(const AutofillType& type) const;

 private:
  const AutofillProfile* profile_;

  
  
  FieldTypeGroup variant_group_;
  size_t variant_;

  DISALLOW_COPY_AND_ASSIGN(AutofillProfileWrapper);
};

class AutofillShippingAddressWrapper : public AutofillProfileWrapper {
 public:
  explicit AutofillShippingAddressWrapper(const AutofillProfile* profile);
  virtual ~AutofillShippingAddressWrapper();

  virtual base::string16 GetInfo(const AutofillType& type) const OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(AutofillShippingAddressWrapper);
};

class AutofillCreditCardWrapper : public DataModelWrapper {
 public:
  explicit AutofillCreditCardWrapper(const CreditCard* card);
  virtual ~AutofillCreditCardWrapper();

  virtual base::string16 GetInfo(const AutofillType& type) const OVERRIDE;
  virtual gfx::Image GetIcon() OVERRIDE;
  virtual bool GetDisplayText(base::string16* vertically_compact,
                              base::string16* horizontally_compact) OVERRIDE;

 private:
  const CreditCard* card_;

  DISALLOW_COPY_AND_ASSIGN(AutofillCreditCardWrapper);
};

class WalletAddressWrapper : public DataModelWrapper {
 public:
  explicit WalletAddressWrapper(const wallet::Address* address);
  virtual ~WalletAddressWrapper();

  virtual base::string16 GetInfo(const AutofillType& type) const OVERRIDE;
  virtual base::string16 GetInfoForDisplay(const AutofillType& type) const
      OVERRIDE;
  virtual bool GetDisplayText(base::string16* vertically_compact,
                              base::string16* horizontally_compact) OVERRIDE;

 private:
  const wallet::Address* address_;

  DISALLOW_COPY_AND_ASSIGN(WalletAddressWrapper);
};

class WalletInstrumentWrapper : public DataModelWrapper {
 public:
  explicit WalletInstrumentWrapper(
      const wallet::WalletItems::MaskedInstrument* instrument);
  virtual ~WalletInstrumentWrapper();

  virtual base::string16 GetInfo(const AutofillType& type) const OVERRIDE;
  virtual base::string16 GetInfoForDisplay(const AutofillType& type) const
      OVERRIDE;
  virtual gfx::Image GetIcon() OVERRIDE;
  virtual bool GetDisplayText(base::string16* vertically_compact,
                              base::string16* horizontally_compact) OVERRIDE;

 private:
  const wallet::WalletItems::MaskedInstrument* instrument_;

  DISALLOW_COPY_AND_ASSIGN(WalletInstrumentWrapper);
};

class FullWalletBillingWrapper : public DataModelWrapper {
 public:
  explicit FullWalletBillingWrapper(wallet::FullWallet* full_wallet);
  virtual ~FullWalletBillingWrapper();

  virtual base::string16 GetInfo(const AutofillType& type) const OVERRIDE;
  virtual bool GetDisplayText(base::string16* vertically_compact,
                              base::string16* horizontally_compact) OVERRIDE;

 private:
  wallet::FullWallet* full_wallet_;

  DISALLOW_COPY_AND_ASSIGN(FullWalletBillingWrapper);
};

class FullWalletShippingWrapper : public DataModelWrapper {
 public:
  explicit FullWalletShippingWrapper(wallet::FullWallet* full_wallet);
  virtual ~FullWalletShippingWrapper();

  virtual base::string16 GetInfo(const AutofillType& type) const OVERRIDE;

 private:
  wallet::FullWallet* full_wallet_;

  DISALLOW_COPY_AND_ASSIGN(FullWalletShippingWrapper);
};

class FieldMapWrapper : public DataModelWrapper {
 public:
  explicit FieldMapWrapper(const FieldValueMap& field_map);
  virtual ~FieldMapWrapper();

  virtual base::string16 GetInfo(const AutofillType& type) const OVERRIDE;

 private:
  const FieldValueMap& field_map_;

  DISALLOW_COPY_AND_ASSIGN(FieldMapWrapper);
};

}  

#endif  
