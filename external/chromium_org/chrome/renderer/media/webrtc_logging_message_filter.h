// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_MEDIA_WEBRTC_LOGGING_MESSAGE_FILTER_H_
#define CHROME_RENDERER_MEDIA_WEBRTC_LOGGING_MESSAGE_FILTER_H_

#include "ipc/ipc_channel_proxy.h"

namespace base {
class MessageLoopProxy;
}

class ChromeWebRtcLogMessageDelegate;

class WebRtcLoggingMessageFilter
    : public IPC::ChannelProxy::MessageFilter {
 public:
  explicit WebRtcLoggingMessageFilter(
      const scoped_refptr<base::MessageLoopProxy>& io_message_loop);

  virtual void AddLogMessage(const std::string& message);
  virtual void LoggingStopped();

  const scoped_refptr<base::MessageLoopProxy>& io_message_loop() {
    return io_message_loop_;
  }

 protected:
  virtual ~WebRtcLoggingMessageFilter();

  scoped_refptr<base::MessageLoopProxy> io_message_loop_;

  
  
  
  
  
  
  
  
  ChromeWebRtcLogMessageDelegate* log_message_delegate_;

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnFilterAdded(IPC::Channel* channel) OVERRIDE;
  virtual void OnFilterRemoved() OVERRIDE;
  virtual void OnChannelClosing() OVERRIDE;

  void CreateLoggingHandler();

  void OnStartLogging();
  void OnStopLogging();
  void Send(IPC::Message* message);

  IPC::Channel* channel_;

  DISALLOW_COPY_AND_ASSIGN(WebRtcLoggingMessageFilter);
};

#endif  
