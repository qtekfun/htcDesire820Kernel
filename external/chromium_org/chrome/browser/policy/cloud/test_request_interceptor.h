// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CLOUD_TEST_REQUEST_INTERCEPTOR_H_
#define CHROME_BROWSER_POLICY_CLOUD_TEST_REQUEST_INTERCEPTOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "policy/proto/device_management_backend.pb.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
}

namespace net {
class NetworkDelegate;
class URLRequest;
class URLRequestJob;
}

namespace policy {

class TestRequestInterceptor {
 public:
  
  
  typedef base::Callback<
      net::URLRequestJob*(net::URLRequest*, net::NetworkDelegate*)> JobCallback;

  
  TestRequestInterceptor(
      const std::string& hostname,
      scoped_refptr<base::SequencedTaskRunner> io_task_runner);
  virtual ~TestRequestInterceptor();

  
  size_t GetPendingSize();

  
  
  void PushJobCallback(const JobCallback& callback);

  
  static JobCallback ErrorJob(int error);

  
  static JobCallback BadRequestJob();

  
  
  
  
  
  
  static JobCallback RegisterJob(
      enterprise_management::DeviceRegisterRequest::Type expected_type,
      bool expect_reregister);

  
  static JobCallback FileJob(const base::FilePath& file_path);

 private:
  class Delegate;

  
  void PostToIOAndWait(const base::Closure& task);

  const std::string hostname_;

  scoped_refptr<base::SequencedTaskRunner> io_task_runner_;

  
  
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(TestRequestInterceptor);
};

}  

#endif  
