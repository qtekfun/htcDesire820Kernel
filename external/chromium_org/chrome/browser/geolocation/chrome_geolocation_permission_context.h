// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GEOLOCATION_CHROME_GEOLOCATION_PERMISSION_CONTEXT_H_
#define CHROME_BROWSER_GEOLOCATION_CHROME_GEOLOCATION_PERMISSION_CONTEXT_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/content_settings/permission_queue_controller.h"
#include "content/public/browser/geolocation_permission_context.h"

class PermissionRequestID;
class Profile;

class ChromeGeolocationPermissionContext
    : public content::GeolocationPermissionContext {
 public:
  explicit ChromeGeolocationPermissionContext(Profile* profile);

  
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

  
  void ShutdownOnUIThread();

 protected:
  virtual ~ChromeGeolocationPermissionContext();

  Profile* profile() const { return profile_; }

  
  
  PermissionQueueController* QueueController();

  
  
  
  
  void NotifyPermissionSet(const PermissionRequestID& id,
                           const GURL& requesting_frame,
                           base::Callback<void(bool)> callback,
                           bool allowed);

  
  
  
  
  
  virtual void DecidePermission(const PermissionRequestID& id,
                                const GURL& requesting_frame,
                                const GURL& embedder,
                                base::Callback<void(bool)> callback);

  
  
  
  
  virtual void PermissionDecided(const PermissionRequestID& id,
                                 const GURL& requesting_frame,
                                 const GURL& embedder,
                                 base::Callback<void(bool)> callback,
                                 bool allowed);

  
  
  virtual PermissionQueueController* CreateQueueController();

 private:
  
  void CancelPendingInfoBarRequest(const PermissionRequestID& id);

  
  Profile* const profile_;
  bool shutting_down_;
  scoped_ptr<PermissionQueueController> permission_queue_controller_;

  DISALLOW_COPY_AND_ASSIGN(ChromeGeolocationPermissionContext);
};

#endif  
