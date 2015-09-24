// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_CRYPTO_MODULE_DELEGATE_NSS_H_
#define CHROME_BROWSER_UI_CRYPTO_MODULE_DELEGATE_NSS_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/synchronization/waitable_event.h"
#include "chrome/browser/ui/crypto_module_password_dialog.h"
#include "crypto/nss_crypto_module_delegate.h"

namespace content {
class ResourceContext;
}

class ChromeNSSCryptoModuleDelegate
    : public crypto::NSSCryptoModuleDelegate {
 public:
  
  
  
  ChromeNSSCryptoModuleDelegate(chrome::CryptoModulePasswordReason reason,
                                const std::string& server);

  virtual ~ChromeNSSCryptoModuleDelegate();

  
  
  
  
  
  bool InitializeSlot(content::ResourceContext* context,
                      const base::Closure& initialization_complete_callback)
      WARN_UNUSED_RESULT;

  
  virtual crypto::ScopedPK11Slot RequestSlot() OVERRIDE;

  
  virtual std::string RequestPassword(const std::string& slot_name,
                                      bool retry,
                                      bool* cancelled) OVERRIDE;

 private:
  void ShowDialog(const std::string& slot_name, bool retry);

  void GotPassword(const std::string& password);

  void DidGetSlot(const base::Closure& callback, crypto::ScopedPK11Slot slot);

  
  const chrome::CryptoModulePasswordReason reason_;
  const std::string server_;

  
  base::WaitableEvent event_;

  
  std::string password_;
  bool cancelled_;

  
  crypto::ScopedPK11Slot slot_;

  DISALLOW_COPY_AND_ASSIGN(ChromeNSSCryptoModuleDelegate);
};

crypto::CryptoModuleBlockingPasswordDelegate*
    CreateCryptoModuleBlockingPasswordDelegate(
        chrome::CryptoModulePasswordReason reason,
        const std::string& server);

#endif  
