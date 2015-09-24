// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_RESOURCE_THROTTLE_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_RESOURCE_THROTTLE_H_

#include "base/memory/weak_ptr.h"
#include "chrome/browser/download/download_request_limiter.h"
#include "content/public/browser/resource_throttle.h"

class GURL;


class DownloadResourceThrottle
    : public content::ResourceThrottle,
      public base::SupportsWeakPtr<DownloadResourceThrottle> {
 public:
  DownloadResourceThrottle(DownloadRequestLimiter* limiter,
                           int render_process_id,
                           int render_view_id,
                           int request_id,
                           const std::string& request_method);

  
  virtual void WillStartRequest(bool* defer) OVERRIDE;
  virtual void WillRedirectRequest(const GURL& new_url, bool* defer) OVERRIDE;
  virtual void WillProcessResponse(bool* defer) OVERRIDE;
  virtual const char* GetNameForLogging() const OVERRIDE;

 private:
  virtual ~DownloadResourceThrottle();

  void WillDownload(bool* defer);
  void ContinueDownload(bool allow);

  
  bool querying_limiter_;

  
  bool request_allowed_;

  
  bool request_deferred_;

  DISALLOW_COPY_AND_ASSIGN(DownloadResourceThrottle);
};

#endif  
