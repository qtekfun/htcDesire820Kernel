// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_ADDRESS_LIST_NET_LOG_PARAM_H_
#define NET_BASE_ADDRESS_LIST_NET_LOG_PARAM_H_
#pragma once

#include "net/base/address_list.h"
#include "net/base/net_log.h"

namespace net {

class AddressListNetLogParam : public NetLog::EventParameters {
 public:
  explicit AddressListNetLogParam(const AddressList& address_list);

  virtual Value* ToValue() const;

 private:
  AddressList address_list_;
};

}  

#endif  
