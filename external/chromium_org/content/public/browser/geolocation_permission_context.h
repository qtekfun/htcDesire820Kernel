// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_GEOLOCATION_PERMISSION_CONTEXT_H_
#define CONTENT_BROWSER_GEOLOCATION_GEOLOCATION_PERMISSION_CONTEXT_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"

class GURL;

namespace content {

class CONTENT_EXPORT GeolocationPermissionContext
    : public base::RefCountedThreadSafe<GeolocationPermissionContext> {
 public:
  
  
  
  virtual void RequestGeolocationPermission(
      int render_process_id,
      int render_view_id,
      int bridge_id,
      const GURL& requesting_frame,
      base::Callback<void(bool)> callback) = 0;

  
  virtual void CancelGeolocationPermissionRequest(
      int render_process_id,
      int render_view_id,
      int bridge_id,
      const GURL& requesting_frame) = 0;

 protected:
   virtual ~GeolocationPermissionContext() {}

 private:
  friend class base::RefCountedThreadSafe<GeolocationPermissionContext>;
};

}  

#endif  
