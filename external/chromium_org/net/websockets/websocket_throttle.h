// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_THROTTLE_H_
#define NET_WEBSOCKETS_WEBSOCKET_THROTTLE_H_

#include <deque>
#include <map>
#include <set>
#include <string>

#include "net/base/ip_endpoint.h"
#include "net/base/net_export.h"

template <typename T> struct DefaultSingletonTraits;

namespace net {

class SocketStream;
class WebSocketJob;

class NET_EXPORT_PRIVATE WebSocketThrottle {
 public:
  
  static WebSocketThrottle* GetInstance();

  
  
  
  
  
  
  bool PutInQueue(WebSocketJob* job);

  
  
  
  void RemoveFromQueue(WebSocketJob* job);

 private:
  typedef std::deque<WebSocketJob*> ConnectingQueue;
  typedef std::map<IPEndPoint, ConnectingQueue> ConnectingAddressMap;

  WebSocketThrottle();
  ~WebSocketThrottle();
  friend struct DefaultSingletonTraits<WebSocketThrottle>;

  
  
  
  
  void WakeupSocketIfNecessary(
      const std::set<WebSocketJob*>& wakeup_candidates);

  
  ConnectingAddressMap addr_map_;

  
  ConnectingQueue queue_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketThrottle);
};

}  

#endif  
