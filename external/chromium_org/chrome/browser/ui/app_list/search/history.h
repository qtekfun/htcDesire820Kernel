// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_HISTORY_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_HISTORY_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/app_list/search/history_data_observer.h"
#include "chrome/browser/ui/app_list/search/history_types.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

namespace content {
class BrowserContext;
}

namespace app_list {

class HistoryData;
class HistoryDataStore;

namespace test {
class SearchHistoryTest;
}

class History : public BrowserContextKeyedService,
                public HistoryDataObserver {
 public:
  explicit History(content::BrowserContext* context);
  virtual ~History();

  
  bool IsReady() const;

  
  void AddLaunchEvent(const std::string& query, const std::string& result_id);

  
  
  scoped_ptr<KnownResults> GetKnownResults(const std::string& query) const;

 private:
  friend class app_list::test::SearchHistoryTest;

  
  virtual void OnHistoryDataLoadedFromStore() OVERRIDE;

  content::BrowserContext* browser_context_;
  scoped_ptr<HistoryData> data_;
  scoped_refptr<HistoryDataStore> store_;
  bool data_loaded_;

  DISALLOW_COPY_AND_ASSIGN(History);
};

}  

#endif  
