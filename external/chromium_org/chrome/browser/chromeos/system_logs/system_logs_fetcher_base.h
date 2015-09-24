// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SYSTEM_LOGS_SYSTEM_LOGS_FETCHER_BASE_H_
#define CHROME_BROWSER_CHROMEOS_SYSTEM_LOGS_SYSTEM_LOGS_FETCHER_BASE_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"

namespace chromeos {

typedef std::map<std::string, std::string> SystemLogsResponse;

typedef base::Callback<void(SystemLogsResponse* response)>
    SysLogsSourceCallback;

typedef base::Callback<void(scoped_ptr<SystemLogsResponse> response)>
    SysLogsFetcherCallback;

class SystemLogsSource {
 public:
  
  virtual void Fetch(const SysLogsSourceCallback& callback) = 0;
  virtual ~SystemLogsSource() {}
};

class SystemLogsFetcherBase
    : public base::SupportsWeakPtr<SystemLogsFetcherBase> {
 public:
  SystemLogsFetcherBase();
  ~SystemLogsFetcherBase();

  void Fetch(const SysLogsFetcherCallback& callback);

 protected:
  
  
  
  
  void AddResponse(SystemLogsResponse* response);

  ScopedVector<SystemLogsSource> data_sources_;
  SysLogsFetcherCallback callback_;

  scoped_ptr<SystemLogsResponse> response_;  
  size_t num_pending_requests_;   

 private:

  DISALLOW_COPY_AND_ASSIGN(SystemLogsFetcherBase);
};

}  

#endif  

