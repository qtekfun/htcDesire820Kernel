// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_EVENT_FILTER_H_
#define EXTENSIONS_COMMON_EVENT_FILTER_H_

#include <map>
#include <set>

#include "base/memory/linked_ptr.h"
#include "components/url_matcher/url_matcher.h"
#include "extensions/common/event_filtering_info.h"
#include "extensions/common/event_matcher.h"

namespace extensions {

class EventFilter {
 public:
  typedef int MatcherID;
  EventFilter();
  ~EventFilter();

  
  
  MatcherID AddEventMatcher(const std::string& event_name,
                            scoped_ptr<EventMatcher> matcher);

  
  EventMatcher* GetEventMatcher(MatcherID id);

  
  
  const std::string& GetEventName(MatcherID id);

  
  std::string RemoveEventMatcher(MatcherID id);

  
  
  
  
  std::set<MatcherID> MatchEvent(const std::string& event_name,
                                 const EventFilteringInfo& event_info,
                                 int routing_id);

  int GetMatcherCountForEvent(const std::string& event_name);

  
  bool IsURLMatcherEmpty() const {
    return url_matcher_.IsEmpty();
  }

 private:
  class EventMatcherEntry {
   public:
    
    
    
    
    EventMatcherEntry(
        scoped_ptr<EventMatcher> event_matcher,
        url_matcher::URLMatcher* url_matcher,
        const url_matcher::URLMatcherConditionSet::Vector& condition_sets);
    ~EventMatcherEntry();

    
    
    
    
    void DontRemoveConditionSetsInDestructor();

    EventMatcher* event_matcher() {
      return event_matcher_.get();
    }

   private:
    scoped_ptr<EventMatcher> event_matcher_;
    
    std::vector<url_matcher::URLMatcherConditionSet::ID> condition_set_ids_;
    url_matcher::URLMatcher* url_matcher_;

    DISALLOW_COPY_AND_ASSIGN(EventMatcherEntry);
  };

  
  typedef std::map<MatcherID, linked_ptr<EventMatcherEntry> > EventMatcherMap;

  
  typedef std::map<std::string, EventMatcherMap> EventMatcherMultiMap;

  
  
  bool CreateConditionSets(
      MatcherID id,
      EventMatcher* matcher,
      url_matcher::URLMatcherConditionSet::Vector* condition_sets);

  bool AddDictionaryAsConditionSet(
      base::DictionaryValue* url_filter,
      url_matcher::URLMatcherConditionSet::Vector* condition_sets);

  url_matcher::URLMatcher url_matcher_;
  EventMatcherMultiMap event_matchers_;

  
  MatcherID next_id_;

  
  url_matcher::URLMatcherConditionSet::ID next_condition_set_id_;

  
  
  
  std::map<url_matcher::URLMatcherConditionSet::ID, MatcherID>
      condition_set_id_to_event_matcher_id_;

  
  std::map<MatcherID, std::string> id_to_event_name_;

  DISALLOW_COPY_AND_ASSIGN(EventFilter);
};

}  

#endif  
