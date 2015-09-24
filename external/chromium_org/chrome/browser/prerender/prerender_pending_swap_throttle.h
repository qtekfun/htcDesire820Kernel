// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_PENDING_SWAP_THROTTLE_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_PENDING_SWAP_THROTTLE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/resource_throttle.h"

namespace net {
class URLRequest;
}

namespace prerender {

class PrerenderTracker;

class PrerenderPendingSwapThrottle
    : public content::ResourceThrottle,
      public base::SupportsWeakPtr<PrerenderPendingSwapThrottle> {
 public:
  PrerenderPendingSwapThrottle(net::URLRequest* request,
                               PrerenderTracker* tracker);

  
  virtual void WillStartRequest(bool* defer) OVERRIDE;

  virtual const char* GetNameForLogging() const OVERRIDE;

  
  
  void Resume();

  
  
  void Cancel();

 private:
  net::URLRequest* request_;
  PrerenderTracker* tracker_;
  bool throttled_;

  DISALLOW_COPY_AND_ASSIGN(PrerenderPendingSwapThrottle);
};

}  

#endif  
