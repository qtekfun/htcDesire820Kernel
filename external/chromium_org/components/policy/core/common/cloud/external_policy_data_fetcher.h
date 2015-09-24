// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_EXTERNAL_POLICY_DATA_FETCHER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_EXTERNAL_POLICY_DATA_FETCHER_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "components/policy/policy_export.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

namespace base {
class SequencedTaskRunner;
}

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}

namespace policy {

class ExternalPolicyDataFetcherBackend;

class POLICY_EXPORT ExternalPolicyDataFetcher {
 public:
  
  enum Result {
    
    SUCCESS,
    
    CONNECTION_INTERRUPTED,
    
    NETWORK_ERROR,
    
    SERVER_ERROR,
    
    CLIENT_ERROR,
    
    HTTP_ERROR,
    
    MAX_SIZE_EXCEEDED,
  };

  
  struct Job;

  
  
  
  
  typedef base::Callback<void(Result, scoped_ptr<std::string>)> FetchCallback;

  
  
  
  ExternalPolicyDataFetcher(
      scoped_refptr<base::SequencedTaskRunner> task_runner,
      scoped_refptr<base::SequencedTaskRunner> io_task_runner,
      const base::WeakPtr<ExternalPolicyDataFetcherBackend>& backend);
  ~ExternalPolicyDataFetcher();

  
  
  
  
  
  Job* StartJob(const GURL& url,
                int64 max_size,
                const FetchCallback& callback);

  
  
  void CancelJob(Job* job);

 private:
  
  void OnJobFinished(const FetchCallback& callback,
                     Job* job,
                     Result result,
                     scoped_ptr<std::string> data);

  
  scoped_refptr<base::SequencedTaskRunner> task_runner_;

  
  
  scoped_refptr<base::SequencedTaskRunner> io_task_runner_;

  
  
  base::WeakPtr<ExternalPolicyDataFetcherBackend> backend_;

  
  typedef std::set<Job*> JobSet;
  JobSet jobs_;

  base::WeakPtrFactory<ExternalPolicyDataFetcher> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ExternalPolicyDataFetcher);
};

class POLICY_EXPORT ExternalPolicyDataFetcherBackend
    : public net::URLFetcherDelegate {
 public:
  
  
  
  
  typedef base::Callback<void(ExternalPolicyDataFetcher::Job*,
                              ExternalPolicyDataFetcher::Result,
                              scoped_ptr<std::string>)> FetchCallback;

  
  
  ExternalPolicyDataFetcherBackend(
      scoped_refptr<base::SequencedTaskRunner> io_task_runner,
      scoped_refptr<net::URLRequestContextGetter> request_context);
  virtual ~ExternalPolicyDataFetcherBackend();

  
  
  scoped_ptr<ExternalPolicyDataFetcher> CreateFrontend(
      scoped_refptr<base::SequencedTaskRunner> task_runner);

  
  
  
  void StartJob(ExternalPolicyDataFetcher::Job* job);

  
  void CancelJob(ExternalPolicyDataFetcher::Job* job,
                 const base::Closure& callback);

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;
  virtual void OnURLFetchDownloadProgress(const net::URLFetcher* source,
                                          int64 current,
                                          int64 total) OVERRIDE;

 private:
  scoped_refptr<base::SequencedTaskRunner> io_task_runner_;
  scoped_refptr<net::URLRequestContextGetter> request_context_;

  
  int last_fetch_id_;

  
  
  typedef std::map<net::URLFetcher*, ExternalPolicyDataFetcher::Job*> JobMap;
  JobMap job_map_;

  base::WeakPtrFactory<ExternalPolicyDataFetcherBackend> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ExternalPolicyDataFetcherBackend);
};


}  

#endif  
