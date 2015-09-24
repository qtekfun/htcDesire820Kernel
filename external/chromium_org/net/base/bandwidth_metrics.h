// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_BANDWIDTH_METRICS_H_
#define NET_BASE_BANDWIDTH_METRICS_H_

#include <list>

#include "base/logging.h"
#include "base/metrics/histogram.h"
#include "base/time/time.h"

namespace net {

class BandwidthMetrics {
 public:
  BandwidthMetrics()
      : num_streams_in_progress_(0),
        num_data_samples_(0),
        data_sum_(0.0),
        bytes_since_last_start_(0) {
  }

  
  double bandwidth() const {
    return data_sum_ / num_data_samples_;
  }

  
  void StartStream() {
    
    
    if (++num_streams_in_progress_ == 1) {
      last_start_ = base::TimeTicks::HighResNow();
      bytes_since_last_start_ = 0;
    }
  }

  
  void StopStream() {
    if (--num_streams_in_progress_ == 0) {
      
      
      
      
      static const int64 kRecordSizeThreshold = 20 * 1024;
      if (bytes_since_last_start_ < kRecordSizeThreshold)
        return;

      base::TimeDelta delta = base::TimeTicks::HighResNow() - last_start_;
      double ms = delta.InMillisecondsF();
      if (ms > 0.0) {
        double kbps = static_cast<double>(bytes_since_last_start_) * 8 / ms;
        ++num_data_samples_;
        data_sum_ += kbps;
        VLOG(1) << "Bandwidth: " << kbps
                << "Kbps (avg " << bandwidth() << "Kbps)";
        int kbps_int = static_cast<int>(kbps);
        UMA_HISTOGRAM_COUNTS_10000("Net.DownloadBandwidth", kbps_int);
      }
    }
  }

  
  void RecordBytes(int bytes) {
    DCHECK(num_streams_in_progress_);
    bytes_since_last_start_ += static_cast<int64>(bytes);
  }

 private:
  int num_streams_in_progress_;   
  
  int num_data_samples_;          
  double data_sum_;               
  int64 bytes_since_last_start_;  
  base::TimeTicks last_start_;    
};

class ScopedBandwidthMetrics {
 public:
  ScopedBandwidthMetrics();
  ~ScopedBandwidthMetrics();

  void StartStream();
  void StopStream();
  void RecordBytes(int bytes);

 private:
  bool started_;
};

}  

#endif  
