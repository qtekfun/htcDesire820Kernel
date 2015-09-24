// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_TCMALLOC_INTERNALS_REQUEST_JOB_H_
#define CONTENT_BROWSER_TCMALLOC_INTERNALS_REQUEST_JOB_H_

#include <map>
#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "base/process/process.h"
#include "build/build_config.h"  
#include "net/url_request/url_request_simple_job.h"

namespace content {

class AboutTcmallocOutputs {
 public:
  
  static AboutTcmallocOutputs* GetInstance();

  
  void SetOutput(const std::string& header, const std::string& output);

  void DumpToHTMLTable(std::string* data);

  
  
  
  void OnStatsForChildProcess(base::ProcessId pid,
                              int process_type,
                              const std::string& output);

 private:
  AboutTcmallocOutputs();
  ~AboutTcmallocOutputs();

  
  
  typedef std::map<std::string, std::string> AboutTcmallocOutputsType;
  AboutTcmallocOutputsType outputs_;

  friend struct DefaultSingletonTraits<AboutTcmallocOutputs>;

  DISALLOW_COPY_AND_ASSIGN(AboutTcmallocOutputs);
};

class TcmallocInternalsRequestJob : public net::URLRequestSimpleJob {
 public:
  TcmallocInternalsRequestJob(net::URLRequest* request,
                              net::NetworkDelegate* network_delegate);

  virtual int GetData(std::string* mime_type,
                      std::string* charset,
                      std::string* data,
                      const net::CompletionCallback& callback) const OVERRIDE;

 protected:
  virtual ~TcmallocInternalsRequestJob() {}

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(TcmallocInternalsRequestJob);
};

}  

#endif  
