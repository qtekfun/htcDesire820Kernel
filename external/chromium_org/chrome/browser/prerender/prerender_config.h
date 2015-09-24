// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_CONFIG_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_CONFIG_H_

#include <string>

#include "base/time/time.h"
#include "ui/gfx/rect.h"

namespace prerender {

struct Config {
  Config();
  ~Config();

  
  size_t max_bytes;

  
  
  size_t max_link_concurrency;

  
  
  size_t max_link_concurrency_per_launcher;

  
  bool rate_limit_enabled;

  
  
  base::TimeDelta max_wait_to_launch;

  
  
  base::TimeDelta time_to_live;

  
  
  
  
  
  
  base::TimeDelta abandon_time_to_live;

  
  bool https_allowed;

  
  
  gfx::Rect default_tab_bounds;

  
  std::string user_agent_override;

  
  bool is_overriding_user_agent;
};

}  

#endif  

