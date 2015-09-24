// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UPDATER_PING_MANAGER_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UPDATER_PING_MANAGER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "url/gurl.h"

namespace net {
class URLRequestContextGetter;
}  

namespace component_updater {

struct CrxUpdateItem;

class PingManager {
 public:
  PingManager(const GURL& ping_url,
              net::URLRequestContextGetter* url_request_context_getter);
  ~PingManager();

  void OnUpdateComplete(const CrxUpdateItem* item);
 private:
  const GURL ping_url_;

  
  net::URLRequestContextGetter* url_request_context_getter_;

  DISALLOW_COPY_AND_ASSIGN(PingManager);
};

}  

#endif  

