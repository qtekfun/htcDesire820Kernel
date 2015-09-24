// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_GEOLOCATION_PROVIDER_H_
#define CONTENT_PUBLIC_BROWSER_GEOLOCATION_PROVIDER_H_

#include "base/callback_forward.h"
#include "content/common/content_export.h"

namespace content {
struct Geoposition;

class CONTENT_EXPORT GeolocationProvider {
 public:
  
  
  static GeolocationProvider* GetInstance();

  typedef base::Callback<void(const Geoposition&)> LocationUpdateCallback;

  
  
  
  
  
  
  virtual void AddLocationUpdateCallback(const LocationUpdateCallback& callback,
                                         bool use_high_accuracy) = 0;

  
  
  virtual bool RemoveLocationUpdateCallback(
      const LocationUpdateCallback& callback) = 0;

  
  
  
  
  virtual void UserDidOptIntoLocationServices() = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  static void OverrideLocationForTesting(
      const Geoposition& position,
      const base::Closure& completion_callback);

 protected:
  virtual~GeolocationProvider() {}
};

}  

#endif  
