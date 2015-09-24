// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INVALIDATION_INVALIDATION_SERVICE_H_
#define CHROME_BROWSER_INVALIDATION_INVALIDATION_SERVICE_H_

#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "sync/notifier/invalidation_util.h"
#include "sync/notifier/invalidator_state.h"

namespace syncer {
class InvalidationHandler;
}  

namespace invalidation {

class InvalidationService : public BrowserContextKeyedService {
 public:
  
  
  
  
  virtual void RegisterInvalidationHandler(
      syncer::InvalidationHandler* handler) = 0;

  
  
  
  
  
  virtual void UpdateRegisteredInvalidationIds(
      syncer::InvalidationHandler* handler,
      const syncer::ObjectIdSet& ids) = 0;

  
  
  
  
  
  virtual void UnregisterInvalidationHandler(
      syncer::InvalidationHandler* handler) = 0;

  
  
  
  virtual syncer::InvalidatorState GetInvalidatorState() const = 0;

  
  
  virtual std::string GetInvalidatorClientId() const = 0;

 protected:
  virtual ~InvalidationService() { }
};

}  

#endif  
