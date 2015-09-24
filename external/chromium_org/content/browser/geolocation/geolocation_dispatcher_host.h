// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_GEOLOCATION_DISPATCHER_HOST_H_
#define CONTENT_BROWSER_GEOLOCATION_GEOLOCATION_DISPATCHER_HOST_H_

#include "content/public/browser/browser_message_filter.h"

namespace content {

class GeolocationPermissionContext;

class GeolocationDispatcherHost : public BrowserMessageFilter {
 public:
  static GeolocationDispatcherHost* New(
      int render_process_id,
      GeolocationPermissionContext* geolocation_permission_context);

  
  
  
  
  
  virtual void PauseOrResume(int render_view_id, bool should_pause) = 0;

 protected:
  GeolocationDispatcherHost();
  virtual ~GeolocationDispatcherHost();

  DISALLOW_COPY_AND_ASSIGN(GeolocationDispatcherHost);
};

}  

#endif  
