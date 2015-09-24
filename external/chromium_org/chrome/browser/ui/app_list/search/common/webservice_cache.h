// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_COMMON_WEBSERVICE_CACHE_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_COMMON_WEBSERVICE_CACHE_H_

#include <utility>

#include "base/basictypes.h"
#include "base/containers/mru_cache.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/ui/app_list/search/common/dictionary_data_store.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

namespace base {
class DictionaryValue;
}

namespace content {
class BrowserContext;
}

namespace app_list {

enum ResultStatus {
  FRESH = 0,
  STALE = 1
};

typedef std::pair<ResultStatus, const base::DictionaryValue*> CacheResult;

class WebserviceCache : public BrowserContextKeyedService,
                        public base::SupportsWeakPtr<WebserviceCache> {
 public:
  enum QueryType {
    WEBSTORE = 0,
    PEOPLE = 1
  };

  explicit WebserviceCache(content::BrowserContext* context);
  virtual ~WebserviceCache();

  
  
  
  
  
  
  const CacheResult Get(QueryType type, const std::string& query);

  
  void Put(QueryType type,
           const std::string& query,
           scoped_ptr<base::DictionaryValue> result);

 private:
  struct Payload {
    Payload(const base::Time& time,
            const base::DictionaryValue* result)
        : time(time), result(result) {}
    Payload() {}

    base::Time time;
    const base::DictionaryValue* result;
  };

  class CacheDeletor {
   public:
    void operator()(Payload& payload);
  };
  typedef base::MRUCacheBase<std::string, Payload, CacheDeletor> Cache;

  
  void OnCacheLoaded(scoped_ptr<base::DictionaryValue>);

  
  
  
  bool PayloadFromDict(const base::DictionaryValue* dict,
                       Payload* payload);

  
  
  
  base::DictionaryValue* DictFromPayload(const Payload& payload);

  
  
  void TrimCache();

  
  std::string PrependType(QueryType type, const std::string& query);

  Cache cache_;
  scoped_refptr<DictionaryDataStore> data_store_;

  bool cache_loaded_;

  DISALLOW_COPY_AND_ASSIGN(WebserviceCache);
};

}  

#endif  
