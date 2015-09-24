// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_CROSS_SITE_REQUEST_MANAGER_H_
#define CONTENT_BROWSER_CROSS_SITE_REQUEST_MANAGER_H_

#include <set>
#include <utility>

#include "base/basictypes.h"
#include "base/synchronization/lock.h"

template <typename T> struct DefaultSingletonTraits;

namespace content {

class CrossSiteRequestManager {
 public:
  
  static CrossSiteRequestManager* GetInstance();

  
  
  
  
  
  bool HasPendingCrossSiteRequest(int renderer_id, int render_view_id);

  
  
  void SetHasPendingCrossSiteRequest(int renderer_id,
                                     int render_view_id,
                                     bool has_pending);

 private:
  friend struct DefaultSingletonTraits<CrossSiteRequestManager>;
  typedef std::set<std::pair<int, int> > RenderViewSet;

  CrossSiteRequestManager();
  ~CrossSiteRequestManager();

  
  
  base::Lock lock_;

  
  
  
  RenderViewSet pending_cross_site_views_;

  DISALLOW_COPY_AND_ASSIGN(CrossSiteRequestManager);
};

}  

#endif  
