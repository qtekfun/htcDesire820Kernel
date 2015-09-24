// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_BROWSER_PLUGIN_BROWSER_PLUGIN_GEOLOCATION_PERMISSION_CONTEXT_H_
#define CONTENT_BROWSER_BROWSER_PLUGIN_BROWSER_PLUGIN_GEOLOCATION_PERMISSION_CONTEXT_H_

#include "content/public/browser/geolocation_permission_context.h"

namespace content {

class BrowserPluginGeolocationPermissionContext :
    public GeolocationPermissionContext {
 public:
  BrowserPluginGeolocationPermissionContext();

  
  virtual void RequestGeolocationPermission(
      int render_process_id,
      int render_view_id,
      int bridge_id,
      const GURL& requesting_frame,
      base::Callback<void(bool)> callback) OVERRIDE;
  virtual void CancelGeolocationPermissionRequest(
      int render_process_id,
      int render_view_id,
      int bridge_id,
      const GURL& requesting_frame) OVERRIDE;

 private:
  virtual ~BrowserPluginGeolocationPermissionContext();

  DISALLOW_COPY_AND_ASSIGN(BrowserPluginGeolocationPermissionContext);
};

}  

#endif  
