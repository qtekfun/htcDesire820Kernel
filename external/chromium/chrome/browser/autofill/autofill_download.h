// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_AUTOFILL_DOWNLOAD_H_
#define CHROME_BROWSER_AUTOFILL_AUTOFILL_DOWNLOAD_H_
#pragma once

#include <stddef.h>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/memory/scoped_vector.h"
#include "base/time.h"
#include "chrome/common/net/url_fetcher.h"

#ifdef ANDROID
#include "android/autofill/url_fetcher_proxy.h"
#endif

class AutofillMetrics;
class FormStructure;
class GURL;
class Profile;

namespace net {
class URLRequestStatus;
}

class AutofillDownloadManager : public URLFetcher::Delegate {
 public:
  enum AutofillRequestType {
    REQUEST_QUERY,
    REQUEST_UPLOAD,
  };

  
  
  class Observer {
   public:
    
    
    virtual void OnLoadedAutofillHeuristics(
        const std::string& heuristic_xml) = 0;
    
    
    
    virtual void OnUploadedAutofillHeuristics(
        const std::string& form_signature) = 0;
    
    
    
    
    virtual void OnHeuristicsRequestError(const std::string& form_signature,
                                          AutofillRequestType request_type,
                                          int http_error) = 0;
   protected:
    virtual ~Observer() {}
  };

  
  explicit AutofillDownloadManager(Profile* profile);
  virtual ~AutofillDownloadManager();

  
  void SetObserver(AutofillDownloadManager::Observer *observer);

  
  
  
  bool StartQueryRequest(const ScopedVector<FormStructure>& forms,
                         const AutofillMetrics& metric_logger);

  
  
  
  
  
  
  bool StartUploadRequest(const FormStructure& form, bool form_was_matched);

  
  
  
  
  
  bool CancelRequest(const std::string& form_signature,
                     AutofillRequestType request_type);

  
  
  
  double GetPositiveUploadRate() const;
  double GetNegativeUploadRate() const;
  
  
  
  
  void SetPositiveUploadRate(double rate);
  void SetNegativeUploadRate(double rate);

 private:
  friend class AutofillDownloadTestHelper;  

  struct FormRequestData;
  typedef std::list<std::pair<std::string, std::string> > QueryRequestCache;

  
  
  
  
  
  
  bool StartRequest(const std::string& form_xml,
                    const FormRequestData& request_data);

  
  
  
  void set_max_form_cache_size(size_t max_form_cache_size) {
    max_form_cache_size_ = max_form_cache_size;
  }

  
  
  void CacheQueryRequest(const std::vector<std::string>& forms_in_query,
                         const std::string& query_data);
  
  
  bool CheckCacheForQueryRequest(const std::vector<std::string>& forms_in_query,
                                 std::string* query_data) const;
  
  std::string GetCombinedSignature(
      const std::vector<std::string>& forms_in_query) const;

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  Profile* profile_;

  
  
  
  std::map<URLFetcher*, FormRequestData> url_fetchers_;
  AutofillDownloadManager::Observer *observer_;

  
  QueryRequestCache cached_forms_;
  size_t max_form_cache_size_;

  
  
  
  base::Time next_query_request_;
  base::Time next_upload_request_;

  
  
  double positive_upload_rate_;
  double negative_upload_rate_;

  
  int fetcher_id_for_unittest_;
};

#endif  

