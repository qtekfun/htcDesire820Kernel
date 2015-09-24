// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_LOCATION_ARBITRATOR_H_
#define CONTENT_BROWSER_GEOLOCATION_LOCATION_ARBITRATOR_H_

#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT LocationArbitrator {
public:
  virtual ~LocationArbitrator() {};

  
  virtual void StartProviders(bool use_high_accuracy) = 0;
  virtual void StopProviders() = 0;

  
  
  
  
  
  virtual void OnPermissionGranted() = 0;

  
  
  virtual bool HasPermissionBeenGranted() const = 0;
};

}  

#endif  
