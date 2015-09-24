// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_FETCHER_FACTORY_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_FETCHER_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

class Profile;
class TemplateURLFetcher;

class TemplateURLFetcherFactory : public BrowserContextKeyedServiceFactory {
 public:
  static TemplateURLFetcher* GetForProfile(Profile* profile);

  static TemplateURLFetcherFactory* GetInstance();

  
  
  
  static void ShutdownForProfile(Profile* profile);

 private:
  friend struct DefaultSingletonTraits<TemplateURLFetcherFactory>;

  TemplateURLFetcherFactory();
  virtual ~TemplateURLFetcherFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;
  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(TemplateURLFetcherFactory);
};

#endif  
