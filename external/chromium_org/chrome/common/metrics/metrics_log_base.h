// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_COMMON_METRICS_METRICS_LOG_BASE_H_
#define CHROME_COMMON_METRICS_METRICS_LOG_BASE_H_

#include <string>

#include "base/basictypes.h"
#include "base/metrics/histogram.h"
#include "base/time/time.h"
#include "chrome/common/metrics/proto/chrome_user_metrics_extension.pb.h"
#include "content/public/common/page_transition_types.h"

class GURL;

namespace base {
class HistogramSamples;
}  

class MetricsLogBase {
 public:
  
  enum LogType {
    INITIAL_LOG,  
    ONGOING_LOG,  
    NO_LOG,       
  };

  
  
  
  MetricsLogBase(const std::string& client_id,
                 int session_id,
                 const std::string& version_string);
  virtual ~MetricsLogBase();

  
  
  static uint64 Hash(const std::string& value);

  
  
  
  
  static int64 GetBuildTime();

  
  
  
  static int64 GetCurrentTime();

  
  void RecordUserAction(const char* key);

  
  void RecordHistogramDelta(const std::string& histogram_name,
                            const base::HistogramSamples& snapshot);

  
  
  void CloseLog();

  
  
  void GetEncodedLog(std::string* encoded_log);

  int num_events() { return num_events_; }

  void set_hardware_class(const std::string& hardware_class) {
    uma_proto_.mutable_system_profile()->mutable_hardware()->set_hardware_class(
        hardware_class);
  }

 protected:
  bool locked() const { return locked_; }

  metrics::ChromeUserMetricsExtension* uma_proto() { return &uma_proto_; }
  const metrics::ChromeUserMetricsExtension* uma_proto() const {
    return &uma_proto_;
  }

  
  int num_events_;  

 private:
  
  // no longer be written to.  It is only used for sanity checking and is
  
  bool locked_;

  
  metrics::ChromeUserMetricsExtension uma_proto_;

  DISALLOW_COPY_AND_ASSIGN(MetricsLogBase);
};

#endif  
