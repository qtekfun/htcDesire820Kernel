// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_PUBLIC_COMMON_GEOPOSITION_H_
#define CONTENT_PUBLIC_COMMON_GEOPOSITION_H_

#include <string>

#include "base/time/time.h"
#include "content/common/content_export.h"

namespace content {

struct CONTENT_EXPORT Geoposition {
 public:
  
  
  enum ErrorCode {
    ERROR_CODE_NONE = 0,  
    ERROR_CODE_PERMISSION_DENIED = 1,
    ERROR_CODE_POSITION_UNAVAILABLE = 2,
    ERROR_CODE_TIMEOUT = 3,
  };

  
  
  Geoposition();

  
  bool Validate() const;

  
  
  
  double latitude;
  
  double longitude;
  
  double altitude;
  
  double accuracy;
  
  double altitude_accuracy;
  
  double heading;
  
  double speed;
  
  
  
  base::Time timestamp;

  
  ErrorCode error_code;
  
  std::string error_message;
};

}  

#endif  
