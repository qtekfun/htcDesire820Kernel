// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_CRYPTO_MODULE_PASSWORD_DIALOG_H_
#define CHROME_BROWSER_UI_CRYPTO_MODULE_PASSWORD_DIALOG_H_
#pragma once

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"

namespace crypto {
class CryptoModuleBlockingPasswordDelegate;
}

namespace net {
class CryptoModule;
typedef std::vector<scoped_refptr<CryptoModule> > CryptoModuleList;
class X509Certificate;
}

namespace browser {

enum CryptoModulePasswordReason {
  kCryptoModulePasswordKeygen,
  kCryptoModulePasswordCertEnrollment,
  kCryptoModulePasswordClientAuth,
  kCryptoModulePasswordListCerts,
  kCryptoModulePasswordCertImport,
  kCryptoModulePasswordCertExport,
};

typedef Callback1<const char*>::Type CryptoModulePasswordCallback;

void ShowCryptoModulePasswordDialog(const std::string& module_name,
                            bool retry,
                            CryptoModulePasswordReason reason,
                            const std::string& server,
                            CryptoModulePasswordCallback* callback);

crypto::CryptoModuleBlockingPasswordDelegate*
    NewCryptoModuleBlockingDialogDelegate(
        CryptoModulePasswordReason reason,
        const std::string& server);

void UnlockSlotsIfNecessary(const net::CryptoModuleList& modules,
                            browser::CryptoModulePasswordReason reason,
                            const std::string& server,
                            Callback0::Type* callback);

void UnlockCertSlotIfNecessary(net::X509Certificate* cert,
                               browser::CryptoModulePasswordReason reason,
                               const std::string& server,
                               Callback0::Type* callback);

}  

#endif  
