// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_EVENT_MATCHER_H_
#define EXTENSIONS_COMMON_EVENT_MATCHER_H_

#include "base/memory/scoped_ptr.h"
#include "base/values.h"

namespace extensions {

class EventFilteringInfo;

extern const char kEventFilterServiceTypeKey[];

class EventMatcher {
 public:
  EventMatcher(scoped_ptr<base::DictionaryValue> filter,
               int routing_id);
  ~EventMatcher();

  
  
  bool MatchNonURLCriteria(const EventFilteringInfo& event_info) const;

  int GetURLFilterCount() const;
  bool GetURLFilter(int i, base::DictionaryValue** url_filter_out);

  std::string GetServiceTypeFilter() const;

  int HasURLFilters() const;

  int GetInstanceID() const;

  int GetRoutingID() const;

  base::DictionaryValue* value() const {
    return filter_.get();
  }

 private:
  
  
  
  
  
  scoped_ptr<base::DictionaryValue> filter_;

  int routing_id_;

  DISALLOW_COPY_AND_ASSIGN(EventMatcher);
};

}  

#endif  
