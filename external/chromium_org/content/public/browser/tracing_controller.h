// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_TRACING_CONTROLLER_H_
#define CONTENT_PUBLIC_BROWSER_TRACING_CONTROLLER_H_

#include <set>
#include <string>

#include "base/callback.h"
#include "content/common/content_export.h"

namespace base {
class FilePath;
};

namespace content {

class TracingController;

class TracingController {
 public:
  enum Options {
    DEFAULT_OPTIONS = 0,
    ENABLE_SYSTRACE = 1 << 0,
    ENABLE_SAMPLING = 1 << 1,
    RECORD_CONTINUOUSLY = 1 << 2,  
  };

  CONTENT_EXPORT static TracingController* GetInstance();

  
  
  
  
  
  
  typedef base::Callback<void(const std::set<std::string>&)>
      GetCategoriesDoneCallback;
  virtual bool GetCategories(
      const GetCategoriesDoneCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  typedef base::Callback<void()> EnableRecordingDoneCallback;
  virtual bool EnableRecording(
      const std::string& category_filter,
      TracingController::Options options,
      const EnableRecordingDoneCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  // Trace data will be written into |result_file_path| if it is not empty, or
  
  
  
  
  // written to any file.
  typedef base::Callback<void(const base::FilePath&)> TracingFileResultCallback;
  virtual bool DisableRecording(const base::FilePath& result_file_path,
                                const TracingFileResultCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  typedef base::Callback<void()> EnableMonitoringDoneCallback;
  virtual bool EnableMonitoring(
      const std::string& category_filter,
      TracingController::Options options,
      const EnableMonitoringDoneCallback& callback) = 0;

  
  
  
  
  typedef base::Callback<void()> DisableMonitoringDoneCallback;
  virtual bool DisableMonitoring(
      const DisableMonitoringDoneCallback& callback) = 0;

  
  virtual void GetMonitoringStatus(bool* out_enabled,
                                   std::string* out_category_filter,
                                   TracingController::Options* out_options) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  // Trace data will be written into |result_file_path| if it is not empty, or
  
  
  
  // written to any file.
  virtual bool CaptureMonitoringSnapshot(
      const base::FilePath& result_file_path,
      const TracingFileResultCallback& callback) = 0;

  
  
  
  typedef base::Callback<void(float)> GetTraceBufferPercentFullCallback;
  virtual bool GetTraceBufferPercentFull(
      const GetTraceBufferPercentFullCallback& callback) = 0;

  
  
  typedef base::Callback<void()> WatchEventCallback;
  virtual bool SetWatchEvent(const std::string& category_name,
                             const std::string& event_name,
                             const WatchEventCallback& callback) = 0;

  
  
  virtual bool CancelWatchEvent() = 0;

 protected:
  virtual ~TracingController() {}
};

}  

#endif  
