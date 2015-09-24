// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_WEB_REQUEST_WEB_REQUEST_PERMISSIONS_H_
#define CHROME_BROWSER_EXTENSIONS_API_WEB_REQUEST_WEB_REQUEST_PERMISSIONS_H_

#include <map>
#include <string>

#include "base/basictypes.h"

class GURL;

namespace extensions {
class InfoMap;
}

namespace net {
class URLRequest;
}

class WebRequestPermissions {
 public:
  
  enum HostPermissionsCheck {
    DO_NOT_CHECK_HOST = 0,    
    REQUIRE_HOST_PERMISSION,  
    REQUIRE_ALL_URLS          
  };

  
  static bool HideRequest(const extensions::InfoMap* extension_info_map,
                          const net::URLRequest* request);

  
  
  static bool CanExtensionAccessURL(
      const extensions::InfoMap* extension_info_map,
      const std::string& extension_id,
      const GURL& url,
      bool crosses_incognito,
      HostPermissionsCheck host_permissions_check);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(WebRequestPermissions);
};

#endif  
