// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_WALLET_ITEMS_H_
#define COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_WALLET_ITEMS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"
#include "components/autofill/content/browser/wallet/required_action.h"
#include "components/autofill/content/browser/wallet/wallet_address.h"
#include "url/gurl.h"

namespace base {
class DictionaryValue;
}

namespace gfx {
class Image;
}

namespace autofill {

class AutofillType;

FORWARD_DECLARE_TEST(WalletInstrumentWrapperTest, GetInfoCreditCardExpMonth);
FORWARD_DECLARE_TEST(WalletInstrumentWrapperTest,
                     GetDisplayTextEmptyWhenExpired);

namespace wallet {

class GaiaAccount;
class WalletItemsTest;

enum AmexPermission {
  AMEX_ALLOWED,
  AMEX_DISALLOWED,
};

class WalletItems {
 public:
  
  
  class MaskedInstrument {
   public:
    enum Type {
      AMEX,
      DISCOVER,
      MAESTRO,
      MASTER_CARD,
      SOLO,
      SWITCH,
      UNKNOWN,  
      VISA,
    };
    enum Status {
      AMEX_NOT_SUPPORTED,
      BILLING_INCOMPLETE,
      DECLINED,
      DISABLED_FOR_THIS_MERCHANT,  
      EXPIRED,
      INAPPLICABLE,  
      PENDING,
      UNSUPPORTED_COUNTRY,
      VALID,
    };

    ~MaskedInstrument();

    
    
    static scoped_ptr<MaskedInstrument>
        CreateMaskedInstrument(const base::DictionaryValue& dictionary);

    bool operator==(const MaskedInstrument& other) const;
    bool operator!=(const MaskedInstrument& other) const;

    
    const gfx::Image& CardIcon() const;

    
    
    base::string16 DisplayName() const;
    base::string16 DisplayNameDetail() const;

    
    base::string16 GetInfo(const AutofillType& type,
                           const std::string& app_locale) const;

    
    base::string16 TypeAndLastFourDigits() const;

    const base::string16& descriptive_name() const { return descriptive_name_; }
    const Type& type() const { return type_; }
    const std::vector<base::string16>& supported_currencies() const {
      return supported_currencies_;
    }
    const base::string16& last_four_digits() const { return last_four_digits_; }
    int expiration_month() const { return expiration_month_; }
    int expiration_year() const { return expiration_year_; }
    const Address& address() const { return *address_; }
    const Status& status() const { return status_; }
    const std::string& object_id() const { return object_id_; }

   private:
    friend class WalletItemsTest;
    friend scoped_ptr<MaskedInstrument> GetTestMaskedInstrumentWithDetails(
        const std::string& id,
        scoped_ptr<Address> address,
        Type type,
        Status status);
    FRIEND_TEST_ALL_PREFIXES(::autofill::WalletInstrumentWrapperTest,
                             GetInfoCreditCardExpMonth);
    FRIEND_TEST_ALL_PREFIXES(::autofill::WalletInstrumentWrapperTest,
                             GetDisplayTextEmptyWhenExpired);
    FRIEND_TEST_ALL_PREFIXES(WalletItemsTest, CreateMaskedInstrument);
    FRIEND_TEST_ALL_PREFIXES(WalletItemsTest, CreateWalletItems);

    MaskedInstrument(const base::string16& descriptive_name,
                     const Type& type,
                     const std::vector<base::string16>& supported_currencies,
                     const base::string16& last_four_digits,
                     int expiration_month,
                     int expiration_year,
                     scoped_ptr<Address> address,
                     const Status& status,
                     const std::string& object_id);

    
    
    base::string16 descriptive_name_;

    
    Type type_;

    
    std::vector<base::string16> supported_currencies_;

    
    base::string16 last_four_digits_;

    
    int expiration_month_;

    
    int expiration_year_;

    
    scoped_ptr<Address> address_;

    
    Status status_;

    
    std::string object_id_;

    DISALLOW_COPY_AND_ASSIGN(MaskedInstrument);
  };

  
  
  class LegalDocument {
   public:
    ~LegalDocument();

    
    static scoped_ptr<LegalDocument>
        CreateLegalDocument(const base::DictionaryValue& dictionary);

    
    
    static scoped_ptr<LegalDocument> CreatePrivacyPolicyDocument();

    bool operator==(const LegalDocument& other) const;
    bool operator!=(const LegalDocument& other) const;

    const std::string& id() { return id_; }
    const GURL& url() const { return url_; }
    const base::string16& display_name() const { return display_name_; }

   private:
    friend class WalletItemsTest;
    FRIEND_TEST_ALL_PREFIXES(WalletItemsTest, CreateLegalDocument);
    FRIEND_TEST_ALL_PREFIXES(WalletItemsTest, CreateWalletItems);
    FRIEND_TEST_ALL_PREFIXES(WalletItemsTest, LegalDocumentUrl);
    FRIEND_TEST_ALL_PREFIXES(WalletItemsTest, LegalDocumentEmptyId);
    LegalDocument(const std::string& id,
                  const base::string16& display_name);
    LegalDocument(const GURL& url,
                  const base::string16& display_name);

    
    
    std::string id_;
    
    GURL url_;
    
    base::string16 display_name_;
    DISALLOW_COPY_AND_ASSIGN(LegalDocument);
  };

  ~WalletItems();

  
  
  
  static scoped_ptr<WalletItems>
      CreateWalletItems(const base::DictionaryValue& dictionary);

  bool operator==(const WalletItems& other) const;
  bool operator!=(const WalletItems& other) const;

  void AddAccount(scoped_ptr<GaiaAccount> account);
  void AddInstrument(scoped_ptr<MaskedInstrument> instrument) {
    DCHECK(instrument);
    instruments_.push_back(instrument.release());
  }
  void AddAddress(scoped_ptr<Address> address) {
    DCHECK(address);
    addresses_.push_back(address.release());
  }
  void AddLegalDocument(scoped_ptr<LegalDocument> legal_document) {
    DCHECK(legal_document);
    legal_documents_.push_back(legal_document.release());
  }

  
  const WalletItems::MaskedInstrument* GetInstrumentById(
      const std::string& object_id) const;

  
  bool HasRequiredAction(RequiredAction action) const;

  
  
  bool SupportsCard(const base::string16& card_number,
                    base::string16* message) const;

  const std::vector<GaiaAccount*>& gaia_accounts() const {
    return gaia_accounts_.get();
  }
  const std::vector<RequiredAction>& required_actions() const {
    return required_actions_;
  }
  const std::string& google_transaction_id() const {
    return google_transaction_id_;
  }
  const std::vector<MaskedInstrument*>& instruments() const {
    return instruments_.get();
  }
  const std::string& default_instrument_id() const {
    return default_instrument_id_;
  }
  const std::vector<Address*>& addresses() const { return addresses_.get(); }
  const std::string& default_address_id() const { return default_address_id_; }
  
  
  std::string ObfuscatedGaiaId() const;
  size_t active_account_index() const { return active_account_index_; }
  const std::vector<LegalDocument*>& legal_documents() const {
    return legal_documents_.get();
  }

 private:
  friend class WalletItemsTest;
  friend scoped_ptr<WalletItems> GetTestWalletItemsWithDetails(
      const std::vector<RequiredAction>& required_actions,
      const std::string& default_instrument_id,
      const std::string& default_address_id,
      AmexPermission amex_permission);
  friend scoped_ptr<WalletItems> GetTestWalletItemsWithDefaultIds(
      RequiredAction action);
  FRIEND_TEST_ALL_PREFIXES(WalletItemsTest, CreateWalletItems);
  FRIEND_TEST_ALL_PREFIXES(WalletItemsTest,
                           CreateWalletItemsWithRequiredActions);

  WalletItems(const std::vector<RequiredAction>& required_actions,
              const std::string& google_transaction_id,
              const std::string& default_instrument_id,
              const std::string& default_address_id,
              AmexPermission amex_permission);

  
  
  std::vector<RequiredAction> required_actions_;

  
  std::string google_transaction_id_;

  
  std::string default_instrument_id_;

  
  std::string default_address_id_;

  
  
  size_t active_account_index_;

  
  ScopedVector<GaiaAccount> gaia_accounts_;

  
  ScopedVector<MaskedInstrument> instruments_;

  
  ScopedVector<Address> addresses_;

  
  ScopedVector<LegalDocument> legal_documents_;

  
  AmexPermission amex_permission_;

  DISALLOW_COPY_AND_ASSIGN(WalletItems);
};

}  
}  

#endif  
