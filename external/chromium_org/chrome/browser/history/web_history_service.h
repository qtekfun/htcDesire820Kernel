// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_WEB_HISTORY_SERVICE_H_
#define CHROME_BROWSER_HISTORY_WEB_HISTORY_SERVICE_H_

#include "base/memory/weak_ptr.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/profiles/profile.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

namespace base {
class DictionaryValue;
}

namespace net {
class URLFetcher;
}

namespace history {

class WebHistoryService : public BrowserContextKeyedService {
 public:
  
  
  
  class Request {
   public:
    virtual ~Request();

    
    
    virtual bool is_pending() = 0;

   protected:
    Request();
  };

  
  
  
  typedef base::Callback<void(Request*, const base::DictionaryValue*)>
      QueryWebHistoryCallback;

  typedef base::Callback<void(Request*, bool success)>
      ExpireWebHistoryCallback;

  explicit WebHistoryService(Profile* profile);
  virtual ~WebHistoryService();

  
  
  
  
  
  
  scoped_ptr<Request> QueryHistory(
      const base::string16& text_query,
      const QueryOptions& options,
      const QueryWebHistoryCallback& callback);

  
  
  
  
  scoped_ptr<Request> ExpireHistory(
      const std::vector<ExpireHistoryArgs>& expire_list,
      const ExpireWebHistoryCallback& callback);

  
  
  scoped_ptr<Request> ExpireHistoryBetween(
      const std::set<GURL>& restrict_urls,
      base::Time begin_time,
      base::Time end_time,
      const ExpireWebHistoryCallback& callback);

 private:
  
  
  
  static void QueryHistoryCompletionCallback(
      const WebHistoryService::QueryWebHistoryCallback& callback,
      WebHistoryService::Request* request,
      bool success);

  
  
  
  void ExpireHistoryCompletionCallback(
      const WebHistoryService::ExpireWebHistoryCallback& callback,
      WebHistoryService::Request* request,
      bool success);

  Profile* profile_;

  
  
  
  std::string server_version_info_;

  base::WeakPtrFactory<WebHistoryService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(WebHistoryService);
};

}  

#endif  
