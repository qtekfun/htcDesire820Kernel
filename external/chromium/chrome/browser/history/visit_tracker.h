// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_VISIT_TRACKER_H__
#define CHROME_BROWSER_HISTORY_VISIT_TRACKER_H__
#pragma once

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/history/history_types.h"

namespace history {

class VisitTracker {
 public:
  VisitTracker();
  ~VisitTracker();

  

  void AddVisit(const void* host,
                int32 page_id,
                const GURL& url,
                VisitID visit_id);

  
  
  void NotifyRenderProcessHostDestruction(const void* host);

  

  
  
  
  VisitID GetLastVisit(const void* host, int32 page_id, const GURL& url);

 private:
  struct Transition {
    GURL url;          
    int32 page_id;     
    VisitID visit_id;  
  };
  typedef std::vector<Transition> TransitionList;
  typedef std::map<const void*, TransitionList*> HostList;

  
  
  
  void CleanupTransitionList(TransitionList* transitions);

  
  HostList hosts_;

  DISALLOW_COPY_AND_ASSIGN(VisitTracker);
};

}  

#endif  
