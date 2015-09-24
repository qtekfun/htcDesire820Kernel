// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef CHROME_BROWSER_NET_REFERRER_H_
#define CHROME_BROWSER_NET_REFERRER_H_

#include <map>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "net/base/host_port_pair.h"
#include "url/gurl.h"

namespace base {
class Value;
}

namespace chrome_browser_net {

class ReferrerValue {
 public:
  ReferrerValue();

  
  void SetSubresourceUseRate(double rate) { subresource_use_rate_ = rate; }

  base::Time birth_time() const { return birth_time_; }

  
  
  void SubresourceIsNeeded();

  
  
  
  
  void ReferrerWasObserved();

  int64 navigation_count() const { return navigation_count_; }
  double subresource_use_rate() const { return subresource_use_rate_; }

  int64 preconnection_count() const { return preconnection_count_; }
  void IncrementPreconnectionCount() { ++preconnection_count_; }

  int64 preresolution_count() const { return preresolution_count_; }
  void preresolution_increment() { ++preresolution_count_; }

  
  
  bool Trim(double reduce_rate, double threshold);

 private:
  const base::Time birth_time_;

  
  int64 navigation_count_;

  
  
  int64 preconnection_count_;

  
  
  int64 preresolution_count_;

  
  
  double subresource_use_rate_;
};

typedef std::map<GURL, ReferrerValue> SubresourceMap;

class Referrer : public SubresourceMap {
 public:
  Referrer();
  void IncrementUseCount() { ++use_count_; }
  int64 use_count() const { return use_count_; }

  
  
  
  void SuggestHost(const GURL& url);

  
  
  
  bool Trim(double reduce_rate, double threshold);

  
  base::Value* Serialize() const;
  void Deserialize(const base::Value& referrers);

 private:
  
  
  
  
  
  
  void DeleteLeastUseful();

  
  
  int64 use_count_;

  
  
  
  
};

}  

#endif  
