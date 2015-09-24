// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_WALLET_SERVICE_URL_H_
#define COMPONENTS_AUTOFILL_CONTENT_BROWSER_WALLET_WALLET_SERVICE_URL_H_

#include <stddef.h>

class GURL;

namespace autofill {
namespace wallet {

GURL GetGetWalletItemsUrl(size_t user_index);
GURL GetGetFullWalletUrl(size_t user_index);
GURL GetManageInstrumentsUrl(size_t user_index);
GURL GetManageAddressesUrl(size_t user_index);
GURL GetAcceptLegalDocumentsUrl(size_t user_index);
GURL GetAuthenticateInstrumentUrl(size_t user_index);
GURL GetSendStatusUrl(size_t user_index);
GURL GetSaveToWalletNoEscrowUrl(size_t user_index);
GURL GetSaveToWalletUrl(size_t user_index);
GURL GetPassiveAuthUrl(size_t user_index);

GURL GetSignInUrl();

GURL GetSignInContinueUrl();

bool IsSignInContinueUrl(const GURL& url, size_t* user_index);

bool IsSignInRelatedUrl(const GURL& url);

bool IsUsingProd();

}  
}  

#endif  
