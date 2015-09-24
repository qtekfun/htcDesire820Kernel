// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_SUGGESTIONS_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_SUGGESTIONS_SOURCE_H_

#include "base/basictypes.h"

class Profile;
class SuggestionsCombiner;

namespace base {
class DictionaryValue;
}

class SuggestionsSource {

 public:
  virtual ~SuggestionsSource() {}

 protected:
  SuggestionsSource() {}

  friend class SuggestionsCombiner;

  
  
  virtual void SetDebug(bool enable) = 0;

  
  
  
  virtual int GetWeight() = 0;

  
  virtual int GetItemCount() = 0;

  
  
  
  
  
  virtual base::DictionaryValue* PopItem() = 0;

  
  
  virtual void FetchItems(Profile* profile) = 0;

  
  
  virtual void SetCombiner(SuggestionsCombiner* combiner) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(SuggestionsSource);
};

#endif  
