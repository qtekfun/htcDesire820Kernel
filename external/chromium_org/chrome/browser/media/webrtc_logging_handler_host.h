// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_WEBRTC_LOGGING_HANDLER_HOST_H_
#define CHROME_BROWSER_MEDIA_WEBRTC_LOGGING_HANDLER_HOST_H_

#include "base/basictypes.h"
#include "base/memory/shared_memory.h"
#include "content/public/browser/browser_message_filter.h"

namespace net {
class URLRequestContextGetter;
}  

class PartialCircularBuffer;
class Profile;
class RenderProcessHost;

class WebRtcLoggingHandlerHost : public content::BrowserMessageFilter {
 public:
  typedef base::Callback<void(bool, const std::string&)> GenericDoneCallback;
  typedef base::Callback<void(bool, const std::string&, const std::string&)>
      UploadDoneCallback;

  explicit WebRtcLoggingHandlerHost(Profile* profile);

  // Sets meta data that will be uploaded along with the log and also written
  
  
  void SetMetaData(const std::map<std::string, std::string>& meta_data,
                   const GenericDoneCallback& callback);

  
  void StartLogging(const GenericDoneCallback& callback);

  
  
  void StopLogging(const GenericDoneCallback& callback);

  
  
  void UploadLog(const UploadDoneCallback& callback);

  
  
  void UploadLogDone();

  
  
  void DiscardLog(const GenericDoneCallback& callback);

  
  
  
  void set_upload_log_on_render_close(bool should_upload) {
    upload_log_on_render_close_ = should_upload;
  }

 private:
  
  
  
  
  
  
  
  
  
  
  enum LoggingState {
    CLOSED,    
    STARTING,  
    STARTED,   
    STOPPING,  
    STOPPED,   
    UPLOADING  
  };

  friend class content::BrowserThread;
  friend class base::DeleteHelper<WebRtcLoggingHandlerHost>;

  virtual ~WebRtcLoggingHandlerHost();

  
  virtual void OnChannelClosing() OVERRIDE;
  virtual void OnDestruct() const OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

  void OnAddLogMessage(const std::string& message);
  void OnLoggingStoppedInRenderer();

  void StartLoggingIfAllowed();
  void DoStartLogging();
  void LogMachineInfo();
  void NotifyLoggingStarted();

  void TriggerUploadLog();

  void FireGenericDoneCallback(GenericDoneCallback* callback,
                               bool success,
                               const std::string& error_message);

  scoped_refptr<net::URLRequestContextGetter> system_request_context_;

  scoped_ptr<unsigned char[]> log_buffer_;
  scoped_ptr<PartialCircularBuffer> circular_buffer_;

  
  Profile* profile_;

  
  
  
  std::map<std::string, std::string> meta_data_;

  
  GenericDoneCallback start_callback_;
  GenericDoneCallback stop_callback_;
  UploadDoneCallback upload_callback_;

  
  
  
  
  LoggingState logging_state_;

  
  bool upload_log_on_render_close_;

  
  
  
  base::SharedMemoryHandle foreign_memory_handle_;

  DISALLOW_COPY_AND_ASSIGN(WebRtcLoggingHandlerHost);
};

#endif  
