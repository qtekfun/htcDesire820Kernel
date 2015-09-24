// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_IMAGE_LOADER_FACTORY_H_
#define CHROME_BROWSER_EXTENSIONS_IMAGE_LOADER_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

namespace content {
class BrowserContext;
}

namespace extensions {

class ImageLoader;

class ImageLoaderFactory : public BrowserContextKeyedServiceFactory {
 public:
  static ImageLoader* GetForBrowserContext(content::BrowserContext* context);

  static ImageLoaderFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<ImageLoaderFactory>;

  ImageLoaderFactory();
  virtual ~ImageLoaderFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const OVERRIDE;
  virtual bool ServiceIsCreatedWithBrowserContext() const OVERRIDE;
  virtual content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const OVERRIDE;
};

}  

#endif  
