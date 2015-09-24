// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PORT_BROWSER_LOCATION_PROVIDER_H_
#define CONTENT_PORT_BROWSER_LOCATION_PROVIDER_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "content/public/common/geoposition.h"

namespace content {

class LocationProvider;

class LocationProvider {
 public:
  virtual ~LocationProvider() {}

  typedef base::Callback<void(const LocationProvider*, const Geoposition&)>
      LocationProviderUpdateCallback;

  
  
  virtual void SetUpdateCallback(
      const LocationProviderUpdateCallback& callback) = 0;

  
  
  
  virtual bool StartProvider(bool high_accuracy) = 0;

  
  
  
  
  
  
  virtual void StopProvider() = 0;

  
  virtual void GetPosition(Geoposition* position) = 0;

  
  
  virtual void RequestRefresh() = 0;

  
  
  
  
  virtual void OnPermissionGranted() = 0;
};

}  

#endif  
