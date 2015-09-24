// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_SIGNIN_GAIA_AUTH_EXTENSION_LOADER_H_
#define CHROME_BROWSER_EXTENSIONS_SIGNIN_GAIA_AUTH_EXTENSION_LOADER_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"

class Profile;

namespace extensions {

class GaiaAuthExtensionLoader : public ProfileKeyedAPI {
 public:
  explicit GaiaAuthExtensionLoader(Profile* profile);
  virtual ~GaiaAuthExtensionLoader();

  
  void LoadIfNeeded();
  
  void UnloadIfNeeded();

  static GaiaAuthExtensionLoader* Get(Profile* profile);

  
  static ProfileKeyedAPIFactory<GaiaAuthExtensionLoader>* GetFactoryInstance();

 private:
  friend class ProfileKeyedAPIFactory<GaiaAuthExtensionLoader>;

  
  virtual void Shutdown() OVERRIDE;

  
  static const char* service_name() {
    return "GaiaAuthExtensionLoader";
  }
  static const bool kServiceHasOwnInstanceInIncognito = true;

  Profile* profile_;
  int load_count_;

  DISALLOW_COPY_AND_ASSIGN(GaiaAuthExtensionLoader);
};

} 

#endif  
