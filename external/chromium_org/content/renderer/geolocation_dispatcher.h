// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_GEOLOCATION_DISPATCHER_H_
#define CONTENT_RENDERER_GEOLOCATION_DISPATCHER_H_

#include "base/memory/scoped_ptr.h"
#include "content/public/renderer/render_view_observer.h"
#include "third_party/WebKit/public/web/WebGeolocationClient.h"
#include "third_party/WebKit/public/web/WebGeolocationController.h"

namespace blink {
class WebGeolocationController;
class WebGeolocationPermissionRequest;
class WebGeolocationPermissionRequestManager;
class WebGeolocationPosition;
}

namespace content {
class RenderViewImpl;
struct Geoposition;

class GeolocationDispatcher : public RenderViewObserver,
                              public blink::WebGeolocationClient {
 public:
  explicit GeolocationDispatcher(RenderViewImpl* render_view);
  virtual ~GeolocationDispatcher();

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual void geolocationDestroyed();
  virtual void startUpdating();
  virtual void stopUpdating();
  virtual void setEnableHighAccuracy(bool enable_high_accuracy);
  virtual void setController(blink::WebGeolocationController* controller);
  virtual bool lastPosition(blink::WebGeolocationPosition& position);
  virtual void requestPermission(
      const blink::WebGeolocationPermissionRequest& permissionRequest);
  virtual void cancelPermissionRequest(
      const blink::WebGeolocationPermissionRequest& permissionRequest);

  
  void OnPermissionSet(int bridge_id, bool is_allowed);

  
  void OnPositionUpdated(const content::Geoposition& geoposition);

  
  
  
  scoped_ptr< blink::WebGeolocationController> controller_;

  scoped_ptr<blink::WebGeolocationPermissionRequestManager>
      pending_permissions_;
  bool enable_high_accuracy_;
  bool updating_;
};

}  

#endif  
