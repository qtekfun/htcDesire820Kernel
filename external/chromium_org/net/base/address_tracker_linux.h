// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_ADDRESS_TRACKER_LINUX_H_
#define NET_BASE_ADDRESS_TRACKER_LINUX_H_

#include <sys/socket.h>  
#define net net_kernel
#include <linux/rtnetlink.h>
#undef net

#include <map>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "net/base/net_util.h"
#include "net/base/network_change_notifier.h"

namespace net {
namespace internal {

class NET_EXPORT_PRIVATE AddressTrackerLinux :
    public base::MessageLoopForIO::Watcher {
 public:
  typedef std::map<IPAddressNumber, struct ifaddrmsg> AddressMap;

  
  
  AddressTrackerLinux(const base::Closure& address_callback,
                      const base::Closure& link_callback);
  virtual ~AddressTrackerLinux();

  
  
  void Init();

  AddressMap GetAddressMap() const;

  
  
  NetworkChangeNotifier::ConnectionType GetCurrentConnectionType();

 private:
  friend class AddressTrackerLinuxTest;

  
  
  
  void ReadMessages(bool* address_changed, bool* link_changed);

  
  
  
  void HandleMessage(const char* buffer,
                     size_t length,
                     bool* address_changed,
                     bool* link_changed);

  
  void AbortAndForceOnline();

  
  virtual void OnFileCanReadWithoutBlocking(int fd) OVERRIDE;
  virtual void OnFileCanWriteWithoutBlocking(int ) OVERRIDE;

  
  void CloseSocket();

  base::Closure address_callback_;
  base::Closure link_callback_;

  int netlink_fd_;
  base::MessageLoopForIO::FileDescriptorWatcher watcher_;

  mutable base::Lock address_map_lock_;
  AddressMap address_map_;

  
  base::hash_set<int> online_links_;

  base::Lock is_offline_lock_;
  bool is_offline_;
  bool is_offline_initialized_;
  base::ConditionVariable is_offline_initialized_cv_;
};

}  
}  

#endif  
