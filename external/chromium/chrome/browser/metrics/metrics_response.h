// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_METRICS_METRICS_RESPONSE_H_
#define CHROME_BROWSER_METRICS_METRICS_RESPONSE_H_
#pragma once

#include <string>

#include "base/basictypes.h"

class MetricsResponse {
 public:
  
  
  explicit MetricsResponse(const std::string& response_xml);

  
  bool valid() { return valid_; }

  
  
  enum CollectorType {
    COLLECTOR_NONE = 0x0,
    COLLECTOR_PROFILE = 0x1,
    COLLECTOR_WINDOW = 0x2,
    COLLECTOR_DOCUMENT = 0x4,
    COLLECTOR_UI = 0x8
  };

  
  
  int collectors() { return collectors_; }

  
  bool collector_active(CollectorType type) { return !!(collectors_ & type); }

  
  
  int events() { return events_; }

  
  
  int interval() { return interval_; }

 private:
  bool valid_;
  int collectors_;
  int events_;
  int interval_;

  DISALLOW_COPY_AND_ASSIGN(MetricsResponse);
};

#endif  
