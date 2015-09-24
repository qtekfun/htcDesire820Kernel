// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_POLICY_CERT_SERVICE_FACTORY_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_POLICY_CERT_SERVICE_FACTORY_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

template <typename T> struct DefaultSingletonTraits;

class PrefRegistrySimple;
class Profile;

namespace policy {

class PolicyCertService;
class PolicyCertVerifier;

class PolicyCertServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  
  
  static PolicyCertService* GetForProfile(Profile* profile);

  
  
  
  
  
  
  
  static scoped_ptr<PolicyCertVerifier> CreateForProfile(Profile* profile);

  static PolicyCertServiceFactory* GetInstance();

  
  
  static void SetUsedPolicyCertificates(const std::string& user_id);
  static void ClearUsedPolicyCertificates(const std::string& user_id);
  static bool UsedPolicyCertificates(const std::string& user_id);

  static void RegisterPrefs(PrefRegistrySimple* local_state);

 private:
  friend struct DefaultSingletonTraits<PolicyCertServiceFactory>;

  PolicyCertServiceFactory();
  virtual ~PolicyCertServiceFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const OVERRIDE;
  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE;
  virtual void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) OVERRIDE;
  virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(PolicyCertServiceFactory);
};

}  

#endif  
