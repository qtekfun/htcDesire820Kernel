/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef LIBRARIES_NACL_IO_EVENT_EMITTER_H_
#define LIBRARIES_NACL_IO_EVENT_EMITTER_H_

#include <stdint.h>

#include <map>
#include <set>

#include "nacl_io/error.h"

#include "sdk_util/macros.h"
#include "sdk_util/ref_object.h"
#include "sdk_util/scoped_ref.h"
#include "sdk_util/simple_lock.h"

namespace nacl_io {

class EventEmitter;
class EventListener;


typedef sdk_util::ScopedRef<EventEmitter> ScopedEventEmitter;
typedef std::map<EventListener*, uint32_t> EventListenerMap_t;

bool operator<(const ScopedEventEmitter& src_a,
               const ScopedEventEmitter& src_b);


class EventEmitter : public sdk_util::RefObject {
 public:
  EventEmitter();

  
  
  uint32_t GetEventStatus() { return event_status_; }

  sdk_util::SimpleLock& GetLock() { return emitter_lock_; }

  
  
  void RaiseEvents_Locked(uint32_t events);

  
  void ClearEvents_Locked(uint32_t events);

  
  
  
  
  void RegisterListener(EventListener* listener, uint32_t events);
  void UnregisterListener(EventListener* listener);
  void RegisterListener_Locked(EventListener* listener, uint32_t events);
  void UnregisterListener_Locked(EventListener* listener);

 private:
  uint32_t event_status_;
  sdk_util::SimpleLock emitter_lock_;
  EventListenerMap_t listeners_;
  DISALLOW_COPY_AND_ASSIGN(EventEmitter);
};

}  


#endif  
