// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_NETWORK_LIST_H_
#define PPAPI_CPP_NETWORK_LIST_H_

#include <string>
#include <vector>

#include "ppapi/c/ppb_network_list.h"
#include "ppapi/cpp/pass_ref.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class NetAddress;

class NetworkList : public Resource {
 public:
  NetworkList();
  NetworkList(PassRef, PP_Resource resource);

  
  static bool IsAvailable();

  
  
  uint32_t GetCount() const;

  
  
  std::string GetName(uint32_t index) const;

  
  
  PP_NetworkList_Type GetType(uint32_t index) const;

  
  
  PP_NetworkList_State GetState(uint32_t index) const;

  
  
  
  int32_t GetIpAddresses(uint32_t index,
                         std::vector<NetAddress>* addresses) const;

  
  
  std::string GetDisplayName(uint32_t index) const;

  
  
  uint32_t GetMTU(uint32_t index) const;
};

}  

#endif  
