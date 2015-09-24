// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_HISTOGRAM_MESSAGE_FILTER_H_
#define CONTENT_CHILD_HISTOGRAM_MESSAGE_FILTER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "ipc/ipc_channel_proxy.h"

namespace base {
class HistogramDeltaSerialization;
class MessageLoopProxy;
}  

namespace content {

class ChildHistogramMessageFilter : public IPC::ChannelProxy::MessageFilter {
 public:
  ChildHistogramMessageFilter();

  
  virtual void OnFilterAdded(IPC::Channel* channel) OVERRIDE;
  virtual void OnFilterRemoved() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void SendHistograms(int sequence_number);

 private:
  typedef std::vector<std::string> HistogramPickledList;

  virtual ~ChildHistogramMessageFilter();

  
  virtual void OnGetChildHistogramData(int sequence_number);

  
  
  void UploadAllHistograms(int sequence_number);

  IPC::Channel* channel_;

  scoped_refptr<base::MessageLoopProxy> io_message_loop_;

  
  scoped_ptr<base::HistogramDeltaSerialization> histogram_delta_serialization_;

  DISALLOW_COPY_AND_ASSIGN(ChildHistogramMessageFilter);
};

}  

#endif  
