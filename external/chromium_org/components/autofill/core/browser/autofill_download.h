// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_DOWNLOAD_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_DOWNLOAD_H_

#include <stddef.h>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/time/time.h"
#include "components/autofill/core/browser/autofill_type.h"
#include "net/url_request/url_fetcher_delegate.h"

class PrefService;

namespace net {
class URLFetcher;
}  

namespace autofill {

class AutofillDriver;
class AutofillMetrics;
class FormStructure;

class AutofillDownloadManager : public net::URLFetcherDelegate {
 public:
  enum AutofillRequestType {
    REQUEST_QUERY,
    REQUEST_UPLOAD,
  };

  
  class Observer {
   public:
    
    
    virtual void OnLoadedServerPredictions(const std::string& response_xml) = 0;

    
    
    
    virtual void OnUploadedPossibleFieldTypes() {}
    
    
    
    
    virtual void OnServerRequestError(const std::string& form_signature,
                                      AutofillRequestType request_type,
                                      int http_error) {}

   protected:
    virtual ~Observer() {}
  };

  
  
  AutofillDownloadManager(AutofillDriver* driver,
                          PrefService* pref_service,
                          Observer* observer);
  virtual ~AutofillDownloadManager();

  
  
  
  bool StartQueryRequest(const std::vector<FormStructure*>& forms,
                         const AutofillMetrics& metric_logger);

  
  
  
  
  
  
  
  bool StartUploadRequest(const FormStructure& form,
                          bool form_was_autofilled,
                          const ServerFieldTypeSet& available_field_types);

 private:
  friend class AutofillDownloadTest;
  FRIEND_TEST_ALL_PREFIXES(AutofillDownloadTest, QueryAndUploadTest);

  static std::string AutofillRequestTypeToString(const AutofillRequestType);

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

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  
  double GetPositiveUploadRate() const;
  double GetNegativeUploadRate() const;
  void SetPositiveUploadRate(double rate);
  void SetNegativeUploadRate(double rate);

  
  
  AutofillDriver* const driver_;  

  
  
  PrefService* const pref_service_;  

  
  
  AutofillDownloadManager::Observer* const observer_;  

  
  
  
  std::map<net::URLFetcher*, FormRequestData> url_fetchers_;

  
  QueryRequestCache cached_forms_;
  size_t max_form_cache_size_;

  
  
  
  base::Time next_query_request_;
  base::Time next_upload_request_;

  
  
  double positive_upload_rate_;
  double negative_upload_rate_;

  
  int fetcher_id_for_unittest_;
};

}  

#endif  
