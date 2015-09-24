// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_HISTOGRAMS_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_HISTOGRAMS_H_

#include <string>

#include "base/time/time.h"
#include "chrome/browser/prerender/prerender_contents.h"
#include "chrome/browser/prerender/prerender_events.h"
#include "chrome/browser/prerender/prerender_final_status.h"
#include "chrome/browser/prerender/prerender_local_predictor.h"
#include "chrome/browser/prerender/prerender_origin.h"
#include "url/gurl.h"

namespace prerender {

class PrerenderHistograms {
 public:
  
  
  PrerenderHistograms();

  
  
  
  void RecordPerceivedPageLoadTime(Origin origin,
                                   base::TimeDelta perceived_page_load_time,
                                   bool was_prerender,
                                   bool was_complete_prerender,
                                   const GURL& url);

  
  
  
  void RecordPercentLoadDoneAtSwapin(Origin origin, double fraction) const;

  
  
  void RecordPageLoadTimeNotSwappedIn(Origin origin,
                                      base::TimeDelta page_load_time,
                                      const GURL& url) const;

  
  
  void RecordTimeUntilUsed(Origin origin,
                           base::TimeDelta time_until_used) const;

  
  void RecordPerSessionCount(Origin origin, int count) const;

  
  void RecordTimeBetweenPrerenderRequests(Origin origin,
                                          base::TimeDelta time) const;

  
  void RecordFinalStatus(Origin origin,
                         uint8 experiment_id,
                         PrerenderContents::MatchCompleteStatus mc_status,
                         FinalStatus final_status) const;

  
  void RecordPrerender(Origin origin, const GURL& url);

  
  void RecordPrerenderStarted(Origin origin) const;

  
  
  void RecordConcurrency(size_t prerender_count) const;

  
  void RecordUsedPrerender(Origin origin) const;

  
  void RecordTimeSinceLastRecentVisit(Origin origin,
                                      base::TimeDelta time) const;

  
  
  void RecordFractionPixelsFinalAtSwapin(Origin origin, double fraction) const;

  
  void RecordEvent(Origin origin, uint8 experiment_id, PrerenderEvent event)
      const;

  
  
  void RecordCookieStatus(Origin origin,
                          uint8 experiment_id,
                          int cookie_status) const;

  void RecordPrerenderPageVisitedStatus(Origin origin,
                                        uint8 experiment_id,
                                        bool visited_before) const;

 private:
  base::TimeTicks GetCurrentTimeTicks() const;

  
  base::TimeDelta GetTimeSinceLastPrerender() const;

  
  
  
  bool WithinWindow() const;

  
  uint8 GetCurrentExperimentId() const;

  
  bool IsOriginExperimentWash() const;

  
  
  uint8 last_experiment_id_;

  
  Origin last_origin_;

  
  
  
  bool origin_experiment_wash_;

  
  
  
  base::TimeTicks last_prerender_seen_time_;

  
  
  
  bool seen_any_pageload_;
  bool seen_pageload_started_after_prerender_;

  DISALLOW_COPY_AND_ASSIGN(PrerenderHistograms);
};

}  

#endif  
