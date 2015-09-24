// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_TRACING_TRACE_MESSAGE_FILTER_H_
#define CONTENT_BROWSER_TRACING_TRACE_MESSAGE_FILTER_H_

#include <string>
#include <vector>

#include "base/debug/trace_event.h"
#include "content/public/browser/browser_message_filter.h"

namespace content {

class TraceMessageFilter : public BrowserMessageFilter {
 public:
  TraceMessageFilter();

  
  virtual void OnChannelClosing() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

  void SendBeginTracing(const std::string& category_filter_str,
                        base::debug::TraceLog::Options options);
  void SendEndTracing();
  void SendEnableMonitoring(const std::string& category_filter_str,
                            base::debug::TraceLog::Options options);
  void SendDisableMonitoring();
  void SendCaptureMonitoringSnapshot();
  void SendGetTraceBufferPercentFull();
  void SendSetWatchEvent(const std::string& category_name,
                         const std::string& event_name);
  void SendCancelWatchEvent();

 protected:
  virtual ~TraceMessageFilter();

 private:
  
  void OnChildSupportsTracing();
  void OnEndTracingAck(const std::vector<std::string>& known_categories);
  void OnCaptureMonitoringSnapshotAcked();
  void OnWatchEventMatched();
  void OnTraceBufferPercentFullReply(float percent_full);
  void OnTraceDataCollected(const std::string& data);
  void OnMonitoringTraceDataCollected(const std::string& data);

  
  bool has_child_;

  
  bool is_awaiting_end_ack_;
  
  bool is_awaiting_capture_monitoring_snapshot_ack_;
  
  bool is_awaiting_buffer_percent_full_ack_;

  DISALLOW_COPY_AND_ASSIGN(TraceMessageFilter);
};

}  

#endif  
