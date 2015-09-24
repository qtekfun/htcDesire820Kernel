// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_URL_REQUEST_AUTOMATION_JOB_H_
#define CHROME_BROWSER_AUTOMATION_URL_REQUEST_AUTOMATION_JOB_H_
#pragma once

#include "base/task.h"
#include "chrome/common/ref_counted_util.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_job.h"

class AutomationResourceMessageFilter;
struct AutomationURLResponse;

namespace net {
class HttpResponseHeaders;
class HttpResponseInfo;
class HostPortPair;
}

namespace IPC {
class Message;
}

class URLRequestAutomationJob : public net::URLRequestJob {
 public:
  URLRequestAutomationJob(net::URLRequest* request, int tab, int request_id,
                          AutomationResourceMessageFilter* filter,
                          bool is_pending);

  
  static bool EnsureProtocolFactoryRegistered();

  static net::URLRequest::ProtocolFactory Factory;

  
  virtual void Start();
  virtual void Kill();
  virtual bool GetMimeType(std::string* mime_type) const;
  virtual bool GetCharset(std::string* charset);
  virtual void GetResponseInfo(net::HttpResponseInfo* info);
  virtual int GetResponseCode() const;
  virtual bool IsRedirectResponse(GURL* location, int* http_status_code);
  virtual uint64 GetUploadProgress() const;
  virtual net::HostPortPair GetSocketAddress() const;

  
  static bool MayFilterMessage(const IPC::Message& message, int* request_id);
  void OnMessage(const IPC::Message& message);

  int id() const {
    return id_;
  }

  int request_id() const {
    return request_id_;
  }

  bool is_pending() const {
    return is_pending_;
  }

  AutomationResourceMessageFilter* message_filter() const {
    return message_filter_;
  }

  
  
  
  void StartPendingJob(int new_tab_handle,
                       AutomationResourceMessageFilter* new_filter);

 protected:
  
  virtual bool ReadRawData(net::IOBuffer* buf, int buf_size, int* bytes_read);

  void StartAsync();
  void Cleanup();
  void DisconnectFromMessageFilter();

  
  void OnRequestStarted(int id, const AutomationURLResponse& response);
  void OnDataAvailable(int id, const std::string& bytes);
  void OnRequestEnd(int id, const net::URLRequestStatus& status);

 private:
  virtual ~URLRequestAutomationJob();

  
  
  void NotifyJobCompletionTask();

  int id_;
  int tab_;
  scoped_refptr<AutomationResourceMessageFilter> message_filter_;

  scoped_refptr<net::IOBuffer> pending_buf_;
  size_t pending_buf_size_;

  std::string mime_type_;
  scoped_refptr<net::HttpResponseHeaders> headers_;
  std::string redirect_url_;
  int redirect_status_;
  int request_id_;

  static int instance_count_;

  static bool is_protocol_factory_registered_;
  
  
  static net::URLRequest::ProtocolFactory* old_http_factory_;
  static net::URLRequest::ProtocolFactory* old_https_factory_;

  
  
  
  bool is_pending_;

  
  
  net::URLRequestStatus request_status_;

  
  net::HostPortPair socket_address_;

  ScopedRunnableMethodFactory<URLRequestAutomationJob> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestAutomationJob);
};

#endif  
