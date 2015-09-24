// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_FRAME_TEST_NET_TEST_AUTOMATION_RESOURCE_MESSAGE_FILTER_H_
#define CHROME_FRAME_TEST_NET_TEST_AUTOMATION_RESOURCE_MESSAGE_FILTER_H_

#include <map>

#include "base/synchronization/lock.h"
#include "chrome/browser/automation/automation_provider.h"
#include "chrome/browser/automation/automation_resource_message_filter.h"
#include "chrome/browser/automation/url_request_automation_job.h"

// URL request tests to run sequentially as they were written while still
class TestAutomationResourceMessageFilter
    : public AutomationResourceMessageFilter {
 public:
  explicit TestAutomationResourceMessageFilter(AutomationProvider* automation);

  virtual bool Send(IPC::Message* message);

  static void OnRequestMessage(URLRequestAutomationJob* job,
                               IPC::Message* msg);

  virtual bool OnMessageReceived(const IPC::Message& message);

  
  virtual bool RegisterRequest(URLRequestAutomationJob* job);
  
  virtual void UnRegisterRequest(URLRequestAutomationJob* job);

 protected:
  AutomationProvider* automation_;
  
  struct RequestJob {
    base::MessageLoop* loop_;
    scoped_refptr<URLRequestAutomationJob> job_;
  };
  typedef std::map<int, RequestJob> RequestMap;
  RequestMap requests_;

  
  base::Lock requests_lock_;
};

#endif  
