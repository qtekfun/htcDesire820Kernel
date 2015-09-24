// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_NOTIFIER_INVALIDATOR_REGISTRAR_H_
#define SYNC_NOTIFIER_INVALIDATOR_REGISTRAR_H_

#include <map>

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "base/threading/thread_checker.h"
#include "sync/base/sync_export.h"
#include "sync/notifier/invalidation_handler.h"
#include "sync/notifier/invalidation_util.h"

namespace invalidation {
class ObjectId;
}  

namespace syncer {

class ObjectIdInvalidationMap;

class SYNC_EXPORT InvalidatorRegistrar {
 public:
  InvalidatorRegistrar();

  
  ~InvalidatorRegistrar();

  
  
  void RegisterHandler(InvalidationHandler* handler);

  
  
  
  void UpdateRegisteredIds(InvalidationHandler* handler,
                           const ObjectIdSet& ids);

  
  
  
  void UnregisterHandler(InvalidationHandler* handler);

  ObjectIdSet GetRegisteredIds(InvalidationHandler* handler) const;

  
  
  ObjectIdSet GetAllRegisteredIds() const;

  
  
  
  
  void DispatchInvalidationsToHandlers(
      const ObjectIdInvalidationMap& invalidation_map);

  
  
  
  void UpdateInvalidatorState(InvalidatorState state);

  
  
  
  InvalidatorState GetInvalidatorState() const;

  bool IsHandlerRegisteredForTest(InvalidationHandler* handler) const;

  
  void DetachFromThreadForTest();

 private:
  typedef std::map<InvalidationHandler*, ObjectIdSet> HandlerIdsMap;

  base::ThreadChecker thread_checker_;
  ObserverList<InvalidationHandler> handlers_;
  HandlerIdsMap handler_to_ids_map_;
  InvalidatorState state_;

  DISALLOW_COPY_AND_ASSIGN(InvalidatorRegistrar);
};

}  

#endif  
