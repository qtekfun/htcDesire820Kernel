// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_CHROME_NETWORK_DATA_SAVING_METRICS_H_
#define CHROME_BROWSER_NET_CHROME_NETWORK_DATA_SAVING_METRICS_H_

#include "base/time/time.h"

namespace net {
class URLRequest;
}

class PrefService;

namespace chrome_browser_net {

enum DataReductionRequestType {
  VIA_DATA_REDUCTION_PROXY,  

  
  
  HTTPS,  
  SHORT_BYPASS,  
  LONG_BYPASS,  
                
  UNKNOWN_TYPE,  
};

DataReductionRequestType GetDataReductionRequestType(
    const net::URLRequest* request);

int64 GetAdjustedOriginalContentLength(
    DataReductionRequestType data_reduction_type,
    int64 original_content_length,
    int64 received_content_length);

#if defined(OS_ANDROID) || defined(OS_IOS)
void UpdateContentLengthPrefsForDataReductionProxy(
    int received_content_length,
    int original_content_length,
    bool with_data_reduction_proxy_enabled,
    DataReductionRequestType data_reduction_type,
    base::Time now, PrefService* prefs);
#endif

void UpdateContentLengthPrefs(
    int received_content_length,
    int original_content_length,
    bool with_data_reduction_proxy_enabled,
    DataReductionRequestType data_reduction_type,
    PrefService* prefs);

}  

#endif  
