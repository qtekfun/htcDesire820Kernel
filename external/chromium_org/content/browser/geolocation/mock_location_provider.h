// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_MOCK_LOCATION_PROVIDER_H_
#define CONTENT_BROWSER_GEOLOCATION_MOCK_LOCATION_PROVIDER_H_


#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread.h"
#include "content/browser/geolocation/location_provider_base.h"
#include "content/public/common/geoposition.h"

namespace content {

class MockLocationProvider : public LocationProviderBase {
 public:
  
  
  explicit MockLocationProvider(MockLocationProvider** self_ref);
  virtual ~MockLocationProvider();

  
  void HandlePositionChanged(const Geoposition& position);

  
  virtual bool StartProvider(bool high_accuracy) OVERRIDE;
  virtual void StopProvider() OVERRIDE;
  virtual void GetPosition(Geoposition* position) OVERRIDE;
  virtual void OnPermissionGranted() OVERRIDE;

  Geoposition position_;
  enum State { STOPPED, LOW_ACCURACY, HIGH_ACCURACY } state_;
  bool is_permission_granted_;
  MockLocationProvider** self_ref_;

  scoped_refptr<base::MessageLoopProxy> provider_loop_;

  
  static MockLocationProvider* instance_;

  DISALLOW_COPY_AND_ASSIGN(MockLocationProvider);
};


LocationProvider* NewMockLocationProvider();
LocationProvider* NewAutoSuccessMockLocationProvider();
LocationProvider* NewAutoFailMockLocationProvider();
LocationProvider* NewAutoSuccessMockNetworkLocationProvider();

}  

#endif  
