/*
 * libjingle
 * Copyright 2004--2006, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products 
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _RATELIMITMANAGER_H_
#define _RATELIMITMANAGER_H_

#include "talk/base/time.h"
#include "talk/base/taskrunner.h"
#include <map>

namespace buzz {


class RateLimitManager {
 public:

  RateLimitManager() { };
  ~RateLimitManager() { 
    for (RateLimitMap::iterator it = rate_limits_.begin();
         it != rate_limits_.end(); ++it) {
      delete it->second;
    }
  };

  
  
  bool VerifyRateLimit(const std::string event_name, int max_count, 
                       int per_x_seconds);

  
  
  bool VerifyRateLimit(const std::string event_name, int max_count, 
                       int per_x_seconds, bool always_update);

 private:
  class RateLimit {
   public:
    RateLimit(int max, int per_x_secs) : counter_(0), max_count_(max),
                                         per_x_seconds_(per_x_secs) {
      event_times_ = new uint32[max_count_];                                                 
      for (int i = 0; i < max_count_; i++) {
        event_times_[i] = 0;
      }
    }

    ~RateLimit() {
      if (event_times_) {
        delete[] event_times_;
      }
    }

    
    bool IsWithinRateLimit() {
      return (talk_base::TimeSince(NextTimeAllowedForCounter()) >= 0);
    }
    
    
    void UpdateRateLimit() {
      event_times_[counter_] = talk_base::Time();
      counter_ = (counter_ + 1) % max_count_;
    }

   private:

    
    uint32 PreviousTimeAtCounter() {
      return event_times_[counter_];
    }

    
    uint32 NextTimeAllowedForCounter() {
      return PreviousTimeAtCounter() + per_x_seconds_ * talk_base::kSecToMsec;
    }

    int counter_; 
    int max_count_; 
    int per_x_seconds_; 
    uint32* event_times_; 
  };

  typedef std::map<const std::string, RateLimit*> RateLimitMap;

  
  RateLimitMap rate_limits_;

  
  RateLimit* GetRateLimit(const std::string event_name);

  
  bool IsWithinRateLimit(const std::string event_name);

  
  void UpdateRateLimit(const std::string event_name, int max_count, 
                       int per_x_seconds); 

};

}

#endif 
