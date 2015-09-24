// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_SEARCH_MODEL_H_
#define CHROME_BROWSER_UI_SEARCH_SEARCH_MODEL_H_

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "chrome/common/search_types.h"

class SearchModelObserver;

enum InstantSupportState {
  INSTANT_SUPPORT_NO,
  INSTANT_SUPPORT_YES,
  INSTANT_SUPPORT_UNKNOWN,
};

class SearchModel {
 public:
  struct State {
    State();
    State(const SearchMode& mode,
          InstantSupportState instant_support,
          bool voice_search_supported);

    bool operator==(const State& rhs) const;

    
    SearchMode mode;

    
    InstantSupportState instant_support;

    
    bool voice_search_supported;
  };

  SearchModel();
  ~SearchModel();

  
  void SetState(const State& state);

  
  const State& state() const { return state_; }

  
  void SetMode(const SearchMode& mode);

  
  const SearchMode& mode() const { return state_.mode; }

  
  
  void SetInstantSupportState(InstantSupportState instant_support);

  
  InstantSupportState instant_support() const {
    return state_.instant_support;
  }

  
  
  void SetVoiceSearchSupported(bool supported);

  
  bool voice_search_supported() const { return state_.voice_search_supported; }

  
  void AddObserver(SearchModelObserver* observer);
  void RemoveObserver(SearchModelObserver* observer);

 private:
  
  State state_;

  
  ObserverList<SearchModelObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(SearchModel);
};

#endif  
