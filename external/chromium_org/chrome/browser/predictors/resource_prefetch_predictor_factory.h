// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCH_PREDICTOR_FACTORY_H_
#define CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCH_PREDICTOR_FACTORY_H_

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

namespace predictors {

class ResourcePrefetchPredictor;

class ResourcePrefetchPredictorFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  static ResourcePrefetchPredictor* GetForProfile(
      content::BrowserContext* context);
  static ResourcePrefetchPredictorFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<ResourcePrefetchPredictorFactory>;

  ResourcePrefetchPredictorFactory();
  virtual ~ResourcePrefetchPredictorFactory();

  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ResourcePrefetchPredictorFactory);
};

}  

#endif  
