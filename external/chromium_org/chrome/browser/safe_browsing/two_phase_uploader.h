// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_TWO_PHASE_UPLOADER_H_
#define CHROME_BROWSER_SAFE_BROWSING_TWO_PHASE_UPLOADER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "net/url_request/url_request_context_getter.h"
#include "url/gurl.h"

namespace base {
class TaskRunner;
}
namespace net {
class URLRequestContextGetter;
}

class TwoPhaseUploaderFactory;

class TwoPhaseUploader : public base::NonThreadSafe {
 public:
  enum State {
    STATE_NONE,
    UPLOAD_METADATA,
    UPLOAD_FILE,
    STATE_SUCCESS,
  };
  typedef base::Callback<void(int64 sent, int64 total)> ProgressCallback;
  typedef base::Callback<void(State state,
                              int net_error,
                              int response_code,
                              const std::string& response_data)> FinishCallback;

  virtual ~TwoPhaseUploader() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  static TwoPhaseUploader* Create(
      net::URLRequestContextGetter* url_request_context_getter,
      base::TaskRunner* file_task_runner,
      const GURL& base_url,
      const std::string& metadata,
      const base::FilePath& file_path,
      const ProgressCallback& progress_callback,
      const FinishCallback& finish_callback);

  
  
  static void RegisterFactory(TwoPhaseUploaderFactory* factory) {
    factory_ = factory;
  }

  
  virtual void Start() = 0;

 private:
  
  
  static TwoPhaseUploaderFactory* factory_;
};

class TwoPhaseUploaderFactory {
 public:
  virtual ~TwoPhaseUploaderFactory() {}

  virtual TwoPhaseUploader* CreateTwoPhaseUploader(
      net::URLRequestContextGetter* url_request_context_getter,
      base::TaskRunner* file_task_runner,
      const GURL& base_url,
      const std::string& metadata,
      const base::FilePath& file_path,
      const TwoPhaseUploader::ProgressCallback& progress_callback,
      const TwoPhaseUploader::FinishCallback& finish_callback) = 0;
};

#endif  
