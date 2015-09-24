// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_ECHO_PRIVATE_API_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_ECHO_PRIVATE_API_H_

#include "base/compiler_specific.h"
#include "chrome/browser/chromeos/ui/echo_dialog_listener.h"
#include "chrome/browser/extensions/chrome_extension_function.h"

class PrefRegistrySimple;

namespace chromeos {

class EchoDialogView;

namespace echo_offer {

void RegisterPrefs(PrefRegistrySimple* registry);

}  

}  

class EchoPrivateGetRegistrationCodeFunction
    : public ChromeSyncExtensionFunction {
 public:
  EchoPrivateGetRegistrationCodeFunction();

 protected:
  virtual ~EchoPrivateGetRegistrationCodeFunction();
  virtual bool RunImpl() OVERRIDE;

 private:
  void GetRegistrationCode(const std::string& type);
  DECLARE_EXTENSION_FUNCTION("echoPrivate.getRegistrationCode",
                             ECHOPRIVATE_GETREGISTRATIONCODE)
};

class EchoPrivateGetOobeTimestampFunction
    : public ChromeAsyncExtensionFunction {
 public:
  EchoPrivateGetOobeTimestampFunction();

 protected:
  virtual ~EchoPrivateGetOobeTimestampFunction();
  virtual bool RunImpl() OVERRIDE;

 private:
  bool GetOobeTimestampOnFileThread();
  DECLARE_EXTENSION_FUNCTION("echoPrivate.getOobeTimestamp",
                             ECHOPRIVATE_GETOOBETIMESTAMP)
};

class EchoPrivateSetOfferInfoFunction : public ChromeSyncExtensionFunction {
 public:
  EchoPrivateSetOfferInfoFunction();

 protected:
  virtual ~EchoPrivateSetOfferInfoFunction();
  virtual bool RunImpl() OVERRIDE;

 private:
  DECLARE_EXTENSION_FUNCTION("echoPrivate.setOfferInfo",
                             ECHOPRIVATE_SETOFFERINFO)
};

class EchoPrivateGetOfferInfoFunction : public ChromeSyncExtensionFunction {
 public:
  EchoPrivateGetOfferInfoFunction();

 protected:
  virtual ~EchoPrivateGetOfferInfoFunction();
  virtual bool RunImpl() OVERRIDE;

 private:
  DECLARE_EXTENSION_FUNCTION("echoPrivate.getOfferInfo",
                             ECHOPRIVATE_GETOFFERINFO)
};

class EchoPrivateGetUserConsentFunction : public ChromeAsyncExtensionFunction,
                                          public chromeos::EchoDialogListener {
 public:
  
  typedef base::Callback<void(chromeos::EchoDialogView* dialog)>
          DialogShownTestCallback;

  EchoPrivateGetUserConsentFunction();

  
  
  static scoped_refptr<EchoPrivateGetUserConsentFunction> CreateForTest(
      const DialogShownTestCallback& dialog_shown_callback);

 protected:
  virtual ~EchoPrivateGetUserConsentFunction();

  virtual bool RunImpl() OVERRIDE;

 private:
  
  virtual void OnAccept() OVERRIDE;
  virtual void OnCancel() OVERRIDE;
  virtual void OnMoreInfoLinkClicked() OVERRIDE;

  
  
  
  
  void CheckRedeemOffersAllowed();
  void OnRedeemOffersAllowedChecked(bool is_allowed);

  
  void Finalize(bool consent);

  
  bool redeem_offers_allowed_;
  
  DialogShownTestCallback dialog_shown_callback_;

  DECLARE_EXTENSION_FUNCTION("echoPrivate.getUserConsent",
                             ECHOPRIVATE_GETUSERCONSENT)
};

#endif  
