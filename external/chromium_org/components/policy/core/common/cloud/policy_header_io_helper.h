// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_POLICY_HEADER_IO_HELPER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_POLICY_HEADER_IO_HELPER_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/sequenced_task_runner.h"
#include "components/policy/policy_export.h"

namespace net {
class URLRequest;
}

namespace policy {

class POLICY_EXPORT PolicyHeaderIOHelper {
 public:
  PolicyHeaderIOHelper(
      const std::string& server_url,
      const std::string& initial_header_value,
      const scoped_refptr<base::SequencedTaskRunner>& task_runner);
  ~PolicyHeaderIOHelper();

  
  
  void AddPolicyHeaders(net::URLRequest* request) const;

  
  
  
  
  void UpdateHeader(const std::string& new_header);

 private:
  
  void UpdateHeaderOnIOThread(const std::string& new_header);

  
  std::string server_url_;

  
  scoped_refptr<base::SequencedTaskRunner> io_task_runner_;

  
  std::string policy_header_;

  DISALLOW_COPY_AND_ASSIGN(PolicyHeaderIOHelper);
};

}  

#endif  
