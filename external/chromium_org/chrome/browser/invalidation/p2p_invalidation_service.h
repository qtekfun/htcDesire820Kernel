// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/threading/non_thread_safe.h"
#include "chrome/browser/invalidation/invalidation_service.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

#ifndef CHROME_BROWSER_INVALIDATION_P2P_INVALIDATION_SERVICE_H_
#define CHROME_BROWSER_INVALIDATION_P2P_INVALIDATION_SERVICE_H_

namespace syncer {
class P2PInvalidator;
}

class Profile;

namespace invalidation {

class P2PInvalidationService
    : public base::NonThreadSafe,
      public InvalidationService {
 public:
  explicit P2PInvalidationService(Profile* profile);
  virtual ~P2PInvalidationService();

  
  virtual void Shutdown() OVERRIDE;

  
  
  virtual void RegisterInvalidationHandler(
      syncer::InvalidationHandler* handler) OVERRIDE;
  virtual void UpdateRegisteredInvalidationIds(
      syncer::InvalidationHandler* handler,
      const syncer::ObjectIdSet& ids) OVERRIDE;
  virtual void UnregisterInvalidationHandler(
      syncer::InvalidationHandler* handler) OVERRIDE;
  virtual syncer::InvalidatorState GetInvalidatorState() const OVERRIDE;
  virtual std::string GetInvalidatorClientId() const OVERRIDE;

  void UpdateCredentials(const std::string& username,
                         const std::string& password);

  void SendInvalidation(const syncer::ObjectIdSet& ids);

 private:
  scoped_ptr<syncer::P2PInvalidator> invalidator_;
  std::string invalidator_id_;

  DISALLOW_COPY_AND_ASSIGN(P2PInvalidationService);
};

}  

#endif  
