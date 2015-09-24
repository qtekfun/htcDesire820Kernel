// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_CLOUD_PRINT_JOB_STATUS_UPDATER_H_
#define CHROME_SERVICE_CLOUD_PRINT_JOB_STATUS_UPDATER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread.h"
#include "chrome/service/cloud_print/cloud_print_url_fetcher.h"
#include "chrome/service/cloud_print/print_system.h"
#include "net/url_request/url_request_status.h"
#include "url/gurl.h"

namespace cloud_print {

class JobStatusUpdater : public base::RefCountedThreadSafe<JobStatusUpdater>,
                         public CloudPrintURLFetcherDelegate {
 public:
  class Delegate {
   public:
    virtual bool OnJobCompleted(JobStatusUpdater* updater) = 0;
    virtual void OnAuthError() = 0;

   protected:
    virtual ~Delegate() {}
  };

  JobStatusUpdater(const std::string& printer_name,
                   const std::string& job_id,
                   PlatformJobId& local_job_id,
                   const GURL& cloud_print_server_url,
                   PrintSystem* print_system,
                   Delegate* delegate);

  
  void UpdateStatus();
  void Stop();

  
  virtual CloudPrintURLFetcher::ResponseAction HandleJSONData(
      const net::URLFetcher* source,
      const GURL& url,
      base::DictionaryValue* json_data,
      bool succeeded) OVERRIDE;
  virtual CloudPrintURLFetcher::ResponseAction OnRequestAuthError() OVERRIDE;
  virtual std::string GetAuthHeader() OVERRIDE;

  base::Time start_time() const {
    return start_time_;
  }

 private:
  friend class base::RefCountedThreadSafe<JobStatusUpdater>;
  virtual ~JobStatusUpdater();

  base::Time start_time_;
  std::string printer_name_;
  std::string job_id_;
  PlatformJobId local_job_id_;
  PrintJobDetails last_job_details_;
  scoped_refptr<CloudPrintURLFetcher> request_;
  GURL cloud_print_server_url_;
  scoped_refptr<PrintSystem> print_system_;
  Delegate* delegate_;
  
  
  bool stopped_;
  DISALLOW_COPY_AND_ASSIGN(JobStatusUpdater);
};

typedef JobStatusUpdater::Delegate JobStatusUpdaterDelegate;

}  

#endif  
