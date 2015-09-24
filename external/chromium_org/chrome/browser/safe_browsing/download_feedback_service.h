// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_DOWNLOAD_FEEDBACK_SERVICE_H_
#define CHROME_BROWSER_SAFE_BROWSING_DOWNLOAD_FEEDBACK_SERVICE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/safe_browsing/download_protection_service.h"
#include "content/public/browser/download_danger_type.h"

namespace base {
class TaskRunner;
}

namespace content {
class DownloadItem;
}

namespace net {
class URLRequestContextGetter;
}

namespace safe_browsing {

class DownloadFeedback;

class DownloadFeedbackService : public base::NonThreadSafe {
 public:
  DownloadFeedbackService(net::URLRequestContextGetter* request_context_getter,
                          base::TaskRunner* file_task_runner);
  ~DownloadFeedbackService();

  
  
  
  
  static void MaybeStorePingsForDownload(
      DownloadProtectionService::DownloadCheckResult result,
      content::DownloadItem* download,
      const std::string& ping,
      const std::string& response);

  
  static bool IsEnabledForDownload(const content::DownloadItem& download);

  
  
  static bool GetPingsForDownloadForTesting(
      const content::DownloadItem& download,
      std::string* ping,
      std::string* response);

  
  static void RecordEligibleDownloadShown(
      content::DownloadDangerType danger_type);

  
  
  
  void BeginFeedbackForDownload(content::DownloadItem* download);

 private:
  static void BeginFeedbackOrDeleteFile(
      const scoped_refptr<base::TaskRunner>& file_task_runner,
      const base::WeakPtr<DownloadFeedbackService>& service,
      const std::string& ping_request,
      const std::string& ping_response,
      const base::FilePath& path);
  void StartPendingFeedback();
  void BeginFeedback(const std::string& ping_request,
                     const std::string& ping_response,
                     const base::FilePath& path);
  void FeedbackComplete();

  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;
  scoped_refptr<base::TaskRunner> file_task_runner_;

  
  
  ScopedVector<DownloadFeedback> active_feedback_;

  base::WeakPtrFactory<DownloadFeedbackService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DownloadFeedbackService);
};
}  

#endif  
