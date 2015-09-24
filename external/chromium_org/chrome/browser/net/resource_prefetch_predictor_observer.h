// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_RESOURCE_PREFETCH_PREDICTOR_OBSERVER_H_
#define CHROME_BROWSER_NET_RESOURCE_PREFETCH_PREDICTOR_OBSERVER_H_

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/predictors/resource_prefetch_predictor.h"
#include "webkit/common/resource_type.h"

namespace net {
class URLRequest;
}

class GURL;

namespace chrome_browser_net {

class ResourcePrefetchPredictorObserver {
 public:
  explicit ResourcePrefetchPredictorObserver(
      predictors::ResourcePrefetchPredictor* predictor);
  ~ResourcePrefetchPredictorObserver();

  
  void OnRequestStarted(net::URLRequest* request,
                        ResourceType::Type resource_type,
                        int child_id,
                        int route_id);
  void OnRequestRedirected(const GURL& redirect_url, net::URLRequest* request);
  void OnResponseStarted(net::URLRequest* request);

 private:
  
  base::WeakPtr<predictors::ResourcePrefetchPredictor> predictor_;

  DISALLOW_COPY_AND_ASSIGN(ResourcePrefetchPredictorObserver);
};

}  

#endif  
