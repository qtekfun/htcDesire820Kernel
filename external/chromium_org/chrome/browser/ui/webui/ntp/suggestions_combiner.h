// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_SUGGESTIONS_COMBINER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_SUGGESTIONS_COMBINER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"

class GURL;
class SuggestionsHandler;
class SuggestionsSource;
class Profile;

namespace base {
class DictionaryValue;
class ListValue;
}

class SuggestionsCombiner {
 public:
  
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    virtual void OnSuggestionsReady() = 0;
  };

  virtual ~SuggestionsCombiner();

  explicit SuggestionsCombiner(SuggestionsCombiner::Delegate* delegate,
                               Profile* profile);

  
  void AddSource(SuggestionsSource* source);

  
  
  
  void EnableDebug(bool enable);

  
  void FetchItems(Profile* profile);

  base::ListValue* GetPageValues();

  
  
  
  void OnItemsReady();

  void SetSuggestionsCount(size_t suggestions_count);

  
  
  static SuggestionsCombiner* Create(SuggestionsCombiner::Delegate* delegate,
                                     Profile* profile);

 private:
  friend class SuggestionsCombinerTest;

  
  
  
  void FillPageValues();

  
  
  void AddExtendedInformation(base::DictionaryValue* page_value);

  
  
  bool IsUrlAlreadyOpen(const GURL& url);

  typedef ScopedVector<SuggestionsSource> SuggestionsSources;

  
  
  SuggestionsSources sources_;

  
  
  int sources_fetching_count_;

  
  SuggestionsCombiner::Delegate* delegate_;

  
  
  size_t suggestions_count_;

  
  scoped_ptr<base::ListValue> page_values_;

  
  
  bool debug_enabled_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(SuggestionsCombiner);
};

#endif  
