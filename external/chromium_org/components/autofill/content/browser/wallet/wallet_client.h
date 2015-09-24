// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_WALLET_CLIENT_H_
#define COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_WALLET_CLIENT_H_

#include <queue>
#include <string>
#include <vector>

#include "base/callback.h"  
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/values.h"
#include "components/autofill/content/browser/wallet/full_wallet.h"
#include "components/autofill/content/browser/wallet/wallet_items.h"
#include "components/autofill/core/browser/autofill_manager_delegate.h"
#include "components/autofill/core/browser/autofill_metrics.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "testing/gtest/include/gtest/gtest_prod.h"
#include "url/gurl.h"

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}

namespace autofill {
namespace wallet {

class Address;
class FullWallet;
class Instrument;
class WalletClientDelegate;


class WalletClient : public net::URLFetcherDelegate {
 public:
  
  enum RiskCapability {
    RELOGIN,
    VERIFY_CVC,
  };

  
  enum ErrorType {
    
    BUYER_ACCOUNT_ERROR,                
                                        
    BUYER_LEGAL_ADDRESS_NOT_SUPPORTED,  
                                        
    UNVERIFIED_KNOW_YOUR_CUSTOMER_STATUS,  
                                           
                                           
    UNSUPPORTED_MERCHANT,               
                                        

    
    BAD_REQUEST,              
                              
    INVALID_PARAMS,           
    UNSUPPORTED_API_VERSION,  
                              

    
    INTERNAL_ERROR,           
    SERVICE_UNAVAILABLE,      

    
    MALFORMED_RESPONSE,       
    NETWORK_ERROR,            
                              

    UNKNOWN_ERROR,            
  };

  struct FullWalletRequest {
   public:
    FullWalletRequest(const std::string& instrument_id,
                      const std::string& address_id,
                      const std::string& google_transaction_id,
                      const std::vector<RiskCapability> risk_capabilities,
                      bool new_wallet_user);
    ~FullWalletRequest();

    
    
    std::string instrument_id;

    
    
    std::string address_id;

    
    std::string google_transaction_id;

    
    std::vector<RiskCapability> risk_capabilities;

    
    bool new_wallet_user;

   private:
    DISALLOW_ASSIGN(FullWalletRequest);
  };

  
  
  
  WalletClient(net::URLRequestContextGetter* context_getter,
               WalletClientDelegate* delegate,
               const GURL& source_url);

  virtual ~WalletClient();

  
  
  
  virtual void GetWalletItems();

  
  
  
  
  
  virtual void AcceptLegalDocuments(
      const std::vector<WalletItems::LegalDocument*>& documents,
      const std::string& google_transaction_id);

  
  
  
  
  virtual void AuthenticateInstrument(
      const std::string& instrument_id,
      const std::string& card_verification_number);

  
  virtual void GetFullWallet(const FullWalletRequest& full_wallet_request);

  
  
  
  
  
  
  virtual void SaveToWallet(
      scoped_ptr<Instrument> instrument,
      scoped_ptr<Address> address,
      const WalletItems::MaskedInstrument* reference_instrument,
      const Address* reference_address);

  bool HasRequestInProgress() const;

  
  void CancelRequests();

  
  void SetUserIndex(size_t user_index);
  size_t user_index() const { return user_index_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(WalletClientTest, PendingRequest);
  FRIEND_TEST_ALL_PREFIXES(WalletClientTest, CancelRequests);

  enum RequestType {
    NO_PENDING_REQUEST,
    ACCEPT_LEGAL_DOCUMENTS,
    AUTHENTICATE_INSTRUMENT,
    GET_FULL_WALLET,
    GET_WALLET_ITEMS,
    SAVE_TO_WALLET,
  };

  
  void DoAcceptLegalDocuments(
      const std::vector<std::string>& document_ids,
      const std::string& google_transaction_id);

  
  
  void MakeWalletRequest(const GURL& url,
                         const std::string& post_body,
                         const std::string& mime_type,
                         RequestType request_type);

  
  void HandleMalformedResponse(RequestType request_type,
                               net::URLFetcher* request);
  void HandleNetworkError(int response_code);
  void HandleWalletError(ErrorType error_type);

  
  void StartNextPendingRequest();

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  void LogRequiredActions(
      const std::vector<RequiredAction>& required_actions) const;

  
  AutofillMetrics::WalletApiCallMetric RequestTypeToUmaMetric(
      RequestType request_type) const;

  
  
  scoped_refptr<net::URLRequestContextGetter> context_getter_;

  
  
  WalletClientDelegate* const delegate_;  

  
  
  size_t user_index_;

  
  GURL source_url_;

  
  scoped_ptr<net::URLFetcher> request_;

  
  
  RequestType request_type_;

  
  std::vector<uint8> one_time_pad_;

  
  std::queue<base::Closure> pending_requests_;

  
  base::Time request_started_timestamp_;

  base::WeakPtrFactory<WalletClient> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(WalletClient);
};

}  
}  

#endif  
