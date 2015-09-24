// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTERNAL_EXTENSION_PROVIDER_IMPL_H_
#define CHROME_BROWSER_EXTENSIONS_EXTERNAL_EXTENSION_PROVIDER_IMPL_H_
#pragma once

#include "chrome/browser/extensions/external_extension_provider_interface.h"

#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/external_extension_loader.h"

class DictionaryValue;
class ExternalExtensionLoader;
class Profile;
class ValueSerializer;
class Version;

class ExternalExtensionProviderImpl
    : public ExternalExtensionProviderInterface {
 public:
  
  
  
  
  
  
  
  ExternalExtensionProviderImpl(
      VisitorInterface* service,
      ExternalExtensionLoader* loader,
      Extension::Location crx_location,
      Extension::Location download_location);

  virtual ~ExternalExtensionProviderImpl();

  
  static void CreateExternalProviders(
      VisitorInterface* service,
      Profile* profile,
      ProviderCollection* provider_list);

  
  
  void SetPrefs(DictionaryValue* prefs);

  
  virtual void VisitRegisteredExtension() const;

  virtual bool HasExtension(const std::string& id) const;

  virtual bool GetExtensionDetails(const std::string& id,
                                   Extension::Location* location,
                                   scoped_ptr<Version>* version) const;

  virtual void ServiceShutdown();

  virtual bool IsReady();

  static const char kLocation[];
  static const char kState[];
  static const char kExternalCrx[];
  static const char kExternalVersion[];
  static const char kExternalUpdateUrl[];

 private:
  
  
  const Extension::Location crx_location_;

  
  
  const Extension::Location download_location_;

 private:
  
  
  VisitorInterface* service_;  

  
  scoped_ptr<DictionaryValue> prefs_;

  
  
  bool ready_;

  
  
  scoped_refptr<ExternalExtensionLoader> loader_;

  DISALLOW_COPY_AND_ASSIGN(ExternalExtensionProviderImpl);
};

#endif  
