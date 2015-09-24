// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTERNAL_PROVIDER_IMPL_H_
#define CHROME_BROWSER_EXTENSIONS_EXTERNAL_PROVIDER_IMPL_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/external_loader.h"
#include "extensions/browser/external_provider_interface.h"
#include "extensions/common/manifest.h"

class Profile;

namespace base {
class DictionaryValue;
class Version;
}

namespace extensions {

class ExternalProviderImpl : public ExternalProviderInterface {
 public:
  
  
  
  
  
  
  
  ExternalProviderImpl(VisitorInterface* service,
                       const scoped_refptr<ExternalLoader>& loader,
                       Profile* profile,
                       Manifest::Location crx_location,
                       Manifest::Location download_location,
                       int creation_flags);

  virtual ~ExternalProviderImpl();

  
  static void CreateExternalProviders(
      VisitorInterface* service,
      Profile* profile,
      ProviderCollection* provider_list);

  
  
  virtual void SetPrefs(base::DictionaryValue* prefs);

  
  virtual void ServiceShutdown() OVERRIDE;
  virtual void VisitRegisteredExtension() OVERRIDE;
  virtual bool HasExtension(const std::string& id) const OVERRIDE;
  virtual bool GetExtensionDetails(
      const std::string& id,
      Manifest::Location* location,
      scoped_ptr<base::Version>* version) const OVERRIDE;

  virtual bool IsReady() const OVERRIDE;

  static const char kExternalCrx[];
  static const char kExternalVersion[];
  static const char kExternalUpdateUrl[];
  static const char kSupportedLocales[];
  static const char kIsBookmarkApp[];
  static const char kIsFromWebstore[];
  static const char kKeepIfPresent[];

  void set_auto_acknowledge(bool auto_acknowledge) {
    auto_acknowledge_ = auto_acknowledge;
  }

 private:
  
  
  const Manifest::Location crx_location_;

  
  
  const Manifest::Location download_location_;

  
  
  VisitorInterface* service_;  

  
  scoped_ptr<base::DictionaryValue> prefs_;

  
  
  bool ready_;

  
  
  scoped_refptr<ExternalLoader> loader_;

  
  Profile* profile_;

  
  
  int creation_flags_;

  
  
  bool auto_acknowledge_;

  DISALLOW_COPY_AND_ASSIGN(ExternalProviderImpl);
};

}  

#endif  
