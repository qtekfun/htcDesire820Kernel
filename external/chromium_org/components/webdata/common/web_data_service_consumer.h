// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEBDATA_COMMON_WEB_DATA_SERVICE_CONSUMER_H_
#define COMPONENTS_WEBDATA_COMMON_WEB_DATA_SERVICE_CONSUMER_H_

#include "components/webdata/common/web_data_results.h"
#include "components/webdata/common/web_data_service_base.h"

class WebDataServiceConsumer {
 public:
  
  
  
  virtual void OnWebDataServiceRequestDone(WebDataServiceBase::Handle h,
                                           const WDTypedResult* result) = 0;

 protected:
  virtual ~WebDataServiceConsumer() {}
};


#endif  
