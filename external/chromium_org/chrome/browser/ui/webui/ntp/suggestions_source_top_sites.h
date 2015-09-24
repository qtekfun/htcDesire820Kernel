// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_SUGGESTIONS_SOURCE_TOP_SITES_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_SUGGESTIONS_SOURCE_TOP_SITES_H_

#include <deque>

#include "base/basictypes.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/visit_filter.h"
#include "chrome/browser/ui/webui/ntp/suggestions_source.h"

class SuggestionsCombiner;
class Profile;

namespace base {
class DictionaryValue;
}

class SuggestionsSourceTopSites : public SuggestionsSource {
 public:
  SuggestionsSourceTopSites();
  virtual ~SuggestionsSourceTopSites();

 protected:
  
  virtual void SetDebug(bool enable) OVERRIDE;
  virtual int GetWeight() OVERRIDE;
  virtual int GetItemCount() OVERRIDE;
  virtual base::DictionaryValue* PopItem() OVERRIDE;
  virtual void FetchItems(Profile* profile) OVERRIDE;
  virtual void SetCombiner(SuggestionsCombiner* combiner) OVERRIDE;

  void OnSuggestionsUrlsAvailable(
      CancelableRequestProvider::Handle handle,
      const history::FilteredURLList& data);

 private:

  
  
  
  static history::VisitFilter::SortingOrder GetSortingOrder();

  
  
  static base::TimeDelta GetFilterWidth();

  
  SuggestionsCombiner* combiner_;

  
  std::deque<base::DictionaryValue*> items_;

  
  bool debug_;

  CancelableRequestConsumer history_consumer_;

  DISALLOW_COPY_AND_ASSIGN(SuggestionsSourceTopSites);
};

#endif  
