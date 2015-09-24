// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_LOAD_NOTIFICATION_DETAILS_H_
#define CONTENT_PUBLIC_BROWSER_LOAD_NOTIFICATION_DETAILS_H_

#include "base/time/time.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/common/page_transition_types.h"
#include "url/gurl.h"

namespace content {

struct LoadNotificationDetails {
  LoadNotificationDetails(const GURL& url,
                          PageTransition origin,
                          base::TimeDelta load_time,
                          NavigationController* controller,
                          int session_index)
      : url(url),
        load_time(load_time),
        session_index(session_index),
        origin(origin),
        controller(controller) {}

  
  GURL url;

  
  base::TimeDelta load_time;

  
  int session_index;

  
  PageTransition origin;

  
  NavigationController* controller;
};

}  

#endif  
