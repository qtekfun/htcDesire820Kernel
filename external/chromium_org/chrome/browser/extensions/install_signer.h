// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_INSTALL_SIGNER_H_
#define CHROME_BROWSER_EXTENSIONS_INSTALL_SIGNER_H_

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "extensions/common/extension.h"

namespace base {
class DictionaryValue;
}

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}

namespace extensions {

struct InstallSignature {
  
  ExtensionIdSet ids;

  
  std::string salt;
  std::string signature;

  
  std::string expire_date;

  
  base::Time timestamp;

  InstallSignature();
  ~InstallSignature();

  
  void ToValue(base::DictionaryValue* value) const;

  static scoped_ptr<InstallSignature> FromValue(
      const base::DictionaryValue& value);
};

class InstallSigner {
 public:
  typedef base::Callback<void(scoped_ptr<InstallSignature>)> SignatureCallback;

  
  
  
  
  InstallSigner(net::URLRequestContextGetter* context_getter,
                const ExtensionIdSet& ids);
  ~InstallSigner();

  
  
  static ExtensionIdSet GetForcedNotFromWebstore();

  
  
  
  void GetSignature(const SignatureCallback& callback);

  
  
  static bool VerifySignature(const InstallSignature& signature);

 private:
  
  
  class FetcherDelegate;

  
  
  void ReportErrorViaCallback();

  
  
  void ParseFetchResponse();

  
  void HandleSignatureResult(const std::string& signature,
                             const std::string& expire_date,
                             const ExtensionIdSet& invalid_ids);

  
  SignatureCallback callback_;

  
  
  ExtensionIdSet ids_;

  
  
  std::string salt_;

  
  net::URLRequestContextGetter* context_getter_;
  scoped_ptr<net::URLFetcher> url_fetcher_;
  scoped_ptr<FetcherDelegate> delegate_;

  
  base::Time request_start_time_;

  DISALLOW_COPY_AND_ASSIGN(InstallSigner);
};

}  

#endif  
