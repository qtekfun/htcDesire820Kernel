// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_NOTIFIER_CACHE_INVALIDATION_PACKET_HANDLER_H_
#define CHROME_BROWSER_SYNC_NOTIFIER_CACHE_INVALIDATION_PACKET_HANDLER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_callback_factory.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"

namespace invalidation {
class InvalidationClient;
class NetworkEndpoint;
}  

namespace talk_base {
class Task;
}  

namespace sync_notifier {

class CacheInvalidationPacketHandler {
 public:
  
  
  
  
  CacheInvalidationPacketHandler(
      base::WeakPtr<talk_base::Task> base_task,
      invalidation::InvalidationClient* invalidation_client);

  
  
  
  ~CacheInvalidationPacketHandler();

  
  
  void HandleOutboundPacket(invalidation::NetworkEndpoint* network_endpoint);

 private:
  FRIEND_TEST(CacheInvalidationPacketHandlerTest, Basic);

  void HandleInboundPacket(const std::string& packet);

  base::NonThreadSafe non_thread_safe_;
  base::ScopedCallbackFactory<CacheInvalidationPacketHandler>
      scoped_callback_factory_;

  base::WeakPtr<talk_base::Task> base_task_;
  invalidation::InvalidationClient* invalidation_client_;

  

  
  int seq_;
  
  const std::string sid_;

  DISALLOW_COPY_AND_ASSIGN(CacheInvalidationPacketHandler);
};

}  

#endif  
