// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INVALIDATION_INVALIDATION_SERVICE_ANDROID_H_
#define CHROME_BROWSER_INVALIDATION_INVALIDATION_SERVICE_ANDROID_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/invalidation/invalidation_service.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "sync/notifier/invalidator_registrar.h"

class Profile;

namespace invalidation {

class InvalidationControllerAndroid;

class InvalidationServiceAndroid
    : public base::NonThreadSafe,
      public InvalidationService,
      public content::NotificationObserver {
 public:
  
  InvalidationServiceAndroid(
      Profile* profile,
      InvalidationControllerAndroid* invalidation_controller);
  virtual ~InvalidationServiceAndroid();

  
  
  
  
  
  virtual void RegisterInvalidationHandler(
      syncer::InvalidationHandler* handler) OVERRIDE;
  virtual void UpdateRegisteredInvalidationIds(
      syncer::InvalidationHandler* handler,
      const syncer::ObjectIdSet& ids) OVERRIDE;
  virtual void UnregisterInvalidationHandler(
      syncer::InvalidationHandler* handler) OVERRIDE;
  virtual syncer::InvalidatorState GetInvalidatorState() const OVERRIDE;
  virtual std::string GetInvalidatorClientId() const OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void TriggerStateChangeForTest(syncer::InvalidatorState state);

 private:
  syncer::InvalidatorRegistrar invalidator_registrar_;
  content::NotificationRegistrar registrar_;
  syncer::InvalidatorState invalidator_state_;
  scoped_ptr<InvalidationControllerAndroid> invalidation_controller_;

  DISALLOW_COPY_AND_ASSIGN(InvalidationServiceAndroid);
};

}  

#endif  
