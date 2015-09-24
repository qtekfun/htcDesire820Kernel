// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_INTERFACE_LIST_H_
#define PPAPI_PROXY_INTERFACE_LIST_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "ppapi/proxy/interface_proxy.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/shared_impl/ppapi_permissions.h"

namespace ppapi {
namespace proxy {

class InterfaceList {
 public:
  InterfaceList();
  ~InterfaceList();

  static InterfaceList* GetInstance();

  
  
  
  
  
  
  
  
  
  
  static PPAPI_PROXY_EXPORT void SetProcessGlobalPermissions(
      const PpapiPermissions& permissions);
  static PPAPI_PROXY_EXPORT void SetSupportsDevChannel(
      bool supports_dev_channel);

  
  
  ApiID GetIDForPPBInterface(const std::string& name) const;
  ApiID GetIDForPPPInterface(const std::string& name) const;

  
  
  InterfaceProxy::Factory GetFactoryForID(ApiID id) const;

  
  
  const void* GetInterfaceForPPB(const std::string& name) const;
  const void* GetInterfaceForPPP(const std::string& name) const;

 private:
  struct InterfaceInfo {
    InterfaceInfo()
        : id(API_ID_NONE),
          iface(NULL),
          required_permission(PERMISSION_NONE) {
    }
    InterfaceInfo(ApiID in_id, const void* in_interface, Permission in_perm)
        : id(in_id),
          iface(in_interface),
          required_permission(in_perm) {
    }

    ApiID id;
    const void* iface;

    
    
    
    Permission required_permission;
  };

  typedef std::map<std::string, InterfaceInfo> NameToInterfaceInfoMap;

  void AddProxy(ApiID id, InterfaceProxy::Factory factory);

  
  
  
  void AddPPB(const char* name, ApiID id, const void* iface,
              Permission permission);
  void AddPPP(const char* name, ApiID id, const void* iface);

  
  
  void AddPPP(const InterfaceProxy::Info* info);

  PpapiPermissions permissions_;

  NameToInterfaceInfoMap name_to_browser_info_;
  NameToInterfaceInfoMap name_to_plugin_info_;

  InterfaceProxy::Factory id_to_factory_[API_ID_COUNT];

  DISALLOW_COPY_AND_ASSIGN(InterfaceList);
};

}  
}  

#endif  

