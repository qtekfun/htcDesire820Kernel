// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_EVICTED_DOMAIN_COOKIE_COUNTER_H_
#define CHROME_BROWSER_NET_EVICTED_DOMAIN_COOKIE_COUNTER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "net/cookies/cookie_monster.h"

namespace net {
class CanonicalCookie;
}  

namespace chrome_browser_net {

class EvictedDomainCookieCounter : public net::CookieMonster::Delegate {
 public:
  
  struct EvictedCookie {
    EvictedCookie(base::Time eviction_time_in,
                  base::Time expiry_time_in,
                  bool is_google_in)
        : eviction_time(eviction_time_in),
          expiry_time(expiry_time_in),
          is_google(is_google_in) {}

    bool is_expired(const base::Time& current_time) const {
      return !expiry_time.is_null() && current_time >= expiry_time;
    }

    base::Time eviction_time;
    base::Time expiry_time;
    bool is_google;
  };

  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void Report(const EvictedCookie& evicted_cookie,
                        const base::Time& reinstatement_time) = 0;

    
    virtual base::Time CurrentTime() const = 0;
  };

  
  explicit EvictedDomainCookieCounter(
      scoped_refptr<net::CookieMonster::Delegate> next_cookie_monster_delegate);

  
  EvictedDomainCookieCounter(
      scoped_refptr<net::CookieMonster::Delegate> next_cookie_monster_delegate,
      scoped_ptr<Delegate> cookie_counter_delegate,
      size_t max_size,
      size_t purge_count);

  
  size_t GetStorageSize() const;

  
  virtual void OnCookieChanged(const net::CanonicalCookie& cookie,
                               bool removed,
                               ChangeCause cause) OVERRIDE;

 private:
  
  typedef std::string EvictedCookieKey;

  
  typedef std::map<EvictedCookieKey, EvictedCookie*> EvictedCookieMap;

  virtual ~EvictedDomainCookieCounter();

  
  
  static EvictedCookieKey GetKey(const net::CanonicalCookie& cookie);

  
  static bool CompareEvictedCookie(
      const EvictedCookieMap::iterator evicted_cookie1,
      const EvictedCookieMap::iterator evicted_cookie2);

  
  
  void GarbageCollect(const base::Time& current_time);

  
  
  void StoreEvictedCookie(const EvictedCookieKey& key,
                          const net::CanonicalCookie& cookie,
                          const base::Time& current_time);

  
  
  void ProcessNewCookie(const EvictedCookieKey& key,
                        const net::CanonicalCookie& cookie,
                        const base::Time& current_time);

  
  scoped_refptr<net::CookieMonster::Delegate> next_cookie_monster_delegate_;

  scoped_ptr<Delegate> cookie_counter_delegate_;

  EvictedCookieMap evicted_cookies_;

  
  const size_t max_size_;

  
  const size_t purge_count_;

  DISALLOW_COPY_AND_ASSIGN(EvictedDomainCookieCounter);
};

}  

#endif  
