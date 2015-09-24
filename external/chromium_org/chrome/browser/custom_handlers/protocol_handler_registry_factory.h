// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CUSTOM_HANDLERS_PROTOCOL_HANDLER_REGISTRY_FACTORY_H_
#define CHROME_BROWSER_CUSTOM_HANDLERS_PROTOCOL_HANDLER_REGISTRY_FACTORY_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class Profile;
class ProtocolHandlerRegistry;
template <typename T> struct DefaultSingletonTraits;

class ProtocolHandlerRegistryFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  
  static ProtocolHandlerRegistryFactory* GetInstance();

  
  
  static ProtocolHandlerRegistry* GetForProfile(Profile* profile);

 protected:
  
  virtual bool ServiceIsCreatedWithBrowserContext() const OVERRIDE;
  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE;
  virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<ProtocolHandlerRegistryFactory>;

  ProtocolHandlerRegistryFactory();
  virtual ~ProtocolHandlerRegistryFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ProtocolHandlerRegistryFactory);
};

#endif  
