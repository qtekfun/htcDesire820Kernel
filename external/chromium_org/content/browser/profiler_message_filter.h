// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_PROFILER_MESSAGE_FILTER_H_
#define CONTENT_BROWSER_PROFILER_MESSAGE_FILTER_H_

#include <string>

#include "content/public/browser/browser_message_filter.h"

namespace tracked_objects {
struct ProcessDataSnapshot;
}

namespace content {

class ProfilerMessageFilter : public BrowserMessageFilter {
 public:
  explicit ProfilerMessageFilter(int process_type);

  
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

 protected:
  virtual ~ProfilerMessageFilter();

 private:
  
  void OnChildProfilerData(
      int sequence_number,
      const tracked_objects::ProcessDataSnapshot& profiler_data);

#if defined(USE_TCMALLOC)
  void OnTcmallocStats(const std::string& output);
#endif

  int process_type_;

  DISALLOW_COPY_AND_ASSIGN(ProfilerMessageFilter);
};

}  

#endif  
