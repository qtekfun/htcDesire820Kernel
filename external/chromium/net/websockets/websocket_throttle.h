// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_THROTTLE_H_
#define NET_WEBSOCKETS_WEBSOCKET_THROTTLE_H_
#pragma once

#include <deque>
#include <string>

#include "base/hash_tables.h"

template <typename T> struct DefaultSingletonTraits;

namespace net {

class SocketStream;
class WebSocketJob;

class WebSocketThrottle {
 public:
  
  static WebSocketThrottle* GetInstance();

  
  
  
  void PutInQueue(WebSocketJob* job);

  
  
  void RemoveFromQueue(WebSocketJob* job);

  
  
  
  
  void WakeupSocketIfNecessary();

 private:
  typedef std::deque<WebSocketJob*> ConnectingQueue;
  typedef base::hash_map<std::string, ConnectingQueue*> ConnectingAddressMap;

  WebSocketThrottle();
  virtual ~WebSocketThrottle();
  friend struct DefaultSingletonTraits<WebSocketThrottle>;

  
  ConnectingAddressMap addr_map_;

  
  ConnectingQueue queue_;
};

}  

#endif  
