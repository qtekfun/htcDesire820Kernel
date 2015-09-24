// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GEOLOCATION_CHROME_GEOLOCATION_PERMISSION_CONTEXT_ANDROID_H_
#define CHROME_BROWSER_GEOLOCATION_CHROME_GEOLOCATION_PERMISSION_CONTEXT_ANDROID_H_

#include "chrome/browser/geolocation/chrome_geolocation_permission_context.h"

class GoogleLocationSettingsHelper;

class ChromeGeolocationPermissionContextAndroid
    : public ChromeGeolocationPermissionContext {
 public:
  explicit ChromeGeolocationPermissionContextAndroid(Profile* profile);

 private:
  friend class ChromeGeolocationPermissionContext;

  virtual ~ChromeGeolocationPermissionContextAndroid();

  
  virtual void DecidePermission(const PermissionRequestID& id,
                                const GURL& requesting_frame,
                                const GURL& embedder,
                                base::Callback<void(bool)> callback) OVERRIDE;

  virtual void PermissionDecided(const PermissionRequestID& id,
                                 const GURL& requesting_frame,
                                 const GURL& embedder,
                                 base::Callback<void(bool)> callback,
                                 bool allowed) OVERRIDE;

  scoped_ptr<GoogleLocationSettingsHelper> google_location_settings_helper_;

  DISALLOW_COPY_AND_ASSIGN(ChromeGeolocationPermissionContextAndroid);
};

#endif  
