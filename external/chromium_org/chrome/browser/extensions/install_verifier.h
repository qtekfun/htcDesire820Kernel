// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_INSTALL_VERIFIER_H_
#define CHROME_BROWSER_EXTENSIONS_INSTALL_VERIFIER_H_

#include <queue>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "extensions/browser/management_policy.h"
#include "extensions/common/extension.h"

namespace net {
class URLRequestContextGetter;
}

namespace extensions {

class ExtensionPrefs;
class InstallSigner;
struct InstallSignature;

// written into the extension preferences and is checked for validity when
class InstallVerifier : public ManagementPolicy::Provider {
 public:
  InstallVerifier(ExtensionPrefs* prefs,
                  net::URLRequestContextGetter* context_getter);
  virtual ~InstallVerifier();

  
  static bool NeedsVerification(const Extension& extension);

  
  
  void Init();

  
  
  
  bool NeedsBootstrap();

  
  base::Time SignatureTimestamp();

  
  typedef base::Callback<void(bool)> AddResultCallback;

  
  
  
  void Add(const std::string& id, const AddResultCallback& callback);
  void AddMany(const ExtensionIdSet& ids,
               const AddResultCallback& callback);

  
  
  void AddProvisional(const ExtensionIdSet& ids);

  
  void Remove(const std::string& id);
  void RemoveMany(const ExtensionIdSet& ids);

  
  virtual std::string GetDebugPolicyProviderName() const OVERRIDE;
  virtual bool MustRemainDisabled(const Extension* extension,
                                  Extension::DisableReason* reason,
                                  base::string16* error) const OVERRIDE;

 private:
  
  
  enum OperationType {
    ADD,
    REMOVE
  };

  
  struct PendingOperation {
    OperationType type;

    
    ExtensionIdSet ids;

    AddResultCallback callback;

    explicit PendingOperation();
    ~PendingOperation();
  };

  
  void GarbageCollect();

  
  bool AllowedByEnterprisePolicy(const std::string& id) const;

  
  bool IsVerified(const std::string& id) const;

  
  
  bool WasInstalledAfterSignature(const std::string& id) const;

  
  
  
  void BeginFetch();

  
  void SaveToPrefs();

  
  void SignatureCallback(scoped_ptr<InstallSignature> signature);

  ExtensionPrefs* prefs_;
  net::URLRequestContextGetter* context_getter_;

  
  
  scoped_ptr<InstallSignature> signature_;

  
  scoped_ptr<InstallSigner> signer_;

  
  std::queue<linked_ptr<PendingOperation> > operation_queue_;

  
  
  ExtensionIdSet provisional_;

  DISALLOW_COPY_AND_ASSIGN(InstallVerifier);
};

}  

#endif  
