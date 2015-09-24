// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NET_URL_INFO_H_
#define CHROME_BROWSER_NET_URL_INFO_H_
#pragma once

#include <string>
#include <vector>

#include "base/time.h"
#include "googleurl/src/gurl.h"
#include "net/base/host_port_pair.h"

namespace chrome_browser_net {

void EnablePredictorDetailedLog(bool enable);

class UrlInfo {
 public:
  
  enum ResolutionMotivation {
    MOUSE_OVER_MOTIVATED,  
    PAGE_SCAN_MOTIVATED,   
    UNIT_TEST_MOTIVATED,
    LINKED_MAX_MOTIVATED,    
    OMNIBOX_MOTIVATED,       
    STARTUP_LIST_MOTIVATED,  
    EARLY_LOAD_MOTIVATED,    
                             
                             

    NO_PREFETCH_MOTIVATION,  

    
    
    
    STATIC_REFERAL_MOTIVATED,  
    LEARNED_REFERAL_MOTIVATED,  
    SELF_REFERAL_MOTIVATED,  

    MAX_MOTIVATED  
  };

  enum DnsProcessingState {
      
      PENDING,       
      QUEUED,        
      ASSIGNED,      
      ASSIGNED_BUT_MARKED,  
      FOUND,         
      NO_SUCH_NAME,  
  };
  static const base::TimeDelta kMaxNonNetworkDnsLookupDuration;
  
  
  static const int kMaxGuaranteedDnsCacheSize = 50;

  typedef std::vector<UrlInfo> UrlInfoTable;

  static const base::TimeDelta kNullDuration;

  
  
  UrlInfo();

  ~UrlInfo();

  
  
  
  
  bool NeedsDnsUpdate();

  
  static void set_cache_expiration(base::TimeDelta time);
  static base::TimeDelta get_cache_expiration();

  
  void SetQueuedState(ResolutionMotivation motivation);
  void SetAssignedState();
  void RemoveFromQueue();
  void SetPendingDeleteState();
  void SetFoundState();
  void SetNoSuchNameState();

  
  void SetUrl(const GURL& url);

  bool was_linked() const { return was_linked_; }

  GURL referring_url() const { return referring_url_; }
  void SetReferringHostname(const GURL& url) {
    referring_url_ = url;
  }

  bool was_found() const { return FOUND == state_; }
  bool was_nonexistent() const { return NO_SUCH_NAME == state_; }
  bool is_assigned() const {
    return ASSIGNED == state_ || ASSIGNED_BUT_MARKED == state_;
  }
  bool is_marked_to_delete() const { return ASSIGNED_BUT_MARKED == state_; }
  const GURL url() const { return url_; }

  bool HasUrl(const GURL& url) const {
    return url_ == url;
  }

  base::TimeDelta resolve_duration() const { return resolve_duration_;}
  base::TimeDelta queue_duration() const { return queue_duration_;}

  void DLogResultsStats(const char* message) const;

  static void GetHtmlTable(const UrlInfoTable& host_infos,
                           const char* description,
                           bool brief,
                           std::string* output);

  
  
  
  void set_time(const base::TimeTicks& time) { time_ = time; }

 private:
  base::TimeDelta GetDuration() {
    base::TimeTicks old_time = time_;
    time_ = base::TimeTicks::Now();
    return time_ - old_time;
  }

  
  bool IsStillCached() const;

  
  
  void SetMotivation(ResolutionMotivation motivation);

  
  std::string GetAsciiMotivation() const;

  
  static base::TimeDelta cache_expiration_duration_;

  
  DnsProcessingState state_;

  
  
  DnsProcessingState old_prequeue_state_;

  GURL url_;  

  
  base::TimeTicks time_;
  
  base::TimeDelta resolve_duration_;
  
  base::TimeDelta queue_duration_;

  int sequence_number_;  
  static int sequence_counter;  

  
  ResolutionMotivation motivation_;

  
  bool was_linked_;

  
  
  
  GURL referring_url_;

  
  
  
};

}  

#endif  
