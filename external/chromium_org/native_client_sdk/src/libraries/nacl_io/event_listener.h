/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef LIBRARIES_NACL_IO_EVENT_LISTENER_H_
#define LIBRARIES_NACL_IO_EVENT_LISTENER_H_

#include <pthread.h>

#include <map>
#include <set>
#include <vector>

#include "nacl_io/error.h"
#include "nacl_io/event_emitter.h"

#include "sdk_util/auto_lock.h"
#include "sdk_util/macros.h"
#include "sdk_util/scoped_ref.h"


namespace nacl_io {

struct EventData {
  
  uint32_t events;
  uint64_t user_data;
};

struct EventRequest {
  ScopedEventEmitter emitter;
  uint32_t filter;
  uint32_t events;
};


class EventListener;
class EventListenerGroup;
class EventListenerSingle;

typedef std::map<EventEmitter*, EventRequest*> EmitterRequestMap_t;

class EventListener {
 public:
  EventListener();
  ~EventListener();

  
  
  virtual void ReceiveEvents(EventEmitter* emitter, uint32_t events) = 0;

 protected:
  pthread_cond_t signal_cond_;
  DISALLOW_COPY_AND_ASSIGN(EventListener);
};


class EventListenerLock : public EventListener {
 public:
  explicit EventListenerLock(EventEmitter* emitter);
  ~EventListenerLock();

  
  
  virtual void ReceiveEvents(EventEmitter* emitter, uint32_t events);

  
  
  
  
  
  
  
  
  Error WaitOnEvent(uint32_t events, int ms_max);

private:
  EventEmitter* emitter_;
  sdk_util::AutoLock* lock_;
  uint32_t events_;
  DISALLOW_COPY_AND_ASSIGN(EventListenerLock);
};


class EventListenerPoll : public EventListener {
 public:
  EventListenerPoll() : EventListener(), signaled_(0) {}

  
  
  virtual void ReceiveEvents(EventEmitter* emitter, uint32_t events);

  
  
  
  
  
  
  Error WaitOnAny(EventRequest* requests, size_t cnt, int ms_max);

 private:
  sdk_util::SimpleLock signal_lock_;
  EmitterRequestMap_t emitters_;
  size_t signaled_;
  DISALLOW_COPY_AND_ASSIGN(EventListenerPoll);
};

}  

#endif  
