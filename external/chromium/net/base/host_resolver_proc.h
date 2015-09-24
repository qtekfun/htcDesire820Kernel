// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_HOST_RESOLVER_PROC_H_
#define NET_BASE_HOST_RESOLVER_PROC_H_
#pragma once

#include <string>

#include "base/memory/ref_counted.h"
#include "net/base/address_family.h"

namespace net {

class AddressList;

class HostResolverProc : public base::RefCountedThreadSafe<HostResolverProc> {
 public:
  explicit HostResolverProc(HostResolverProc* previous);

  
  
  
  
  virtual int Resolve(const std::string& host,
                      AddressFamily address_family,
                      HostResolverFlags host_resolver_flags,
                      AddressList* addrlist,
                      int* os_error) = 0;

 protected:
  friend class base::RefCountedThreadSafe<HostResolverProc>;

  virtual ~HostResolverProc();

  
  int ResolveUsingPrevious(const std::string& host,
                           AddressFamily address_family,
                           HostResolverFlags host_resolver_flags,
                           AddressList* addrlist,
                           int* os_error);

 private:
  friend class HostResolverImpl;
  friend class MockHostResolverBase;
  friend class ScopedDefaultHostResolverProc;

  
  
  void SetPreviousProc(HostResolverProc* proc);

  
  
  void SetLastProc(HostResolverProc* proc);

  
  
  static HostResolverProc* GetLastProc(HostResolverProc* proc);

  
  
  
  
  static HostResolverProc* SetDefault(HostResolverProc* proc);
  static HostResolverProc* GetDefault();

  scoped_refptr<HostResolverProc> previous_proc_;
  static HostResolverProc* default_proc_;

  DISALLOW_COPY_AND_ASSIGN(HostResolverProc);
};

int SystemHostResolverProc(const std::string& host,
                           AddressFamily address_family,
                           HostResolverFlags host_resolver_flags,
                           AddressList* addrlist,
                           int* os_error);

}  

#endif  
