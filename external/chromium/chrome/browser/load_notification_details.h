// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_LOAD_NOTIFICATION_DETAILS_H__
#define CHROME_BROWSER_LOAD_NOTIFICATION_DETAILS_H__
#pragma once

#include "base/basictypes.h"
#include "base/time.h"
#include "content/browser/tab_contents/navigation_controller.h"
#include "content/common/page_transition_types.h"
#include "googleurl/src/gurl.h"

class LoadNotificationDetails {
 public:
  LoadNotificationDetails(const GURL& url,
                          PageTransition::Type origin,
                          base::TimeDelta load_time,
                          NavigationController* controller,
                          int session_index)
      : url_(url),
        load_time_(load_time),
        session_index_(session_index),
        origin_(origin),
        controller_(controller) {}

  ~LoadNotificationDetails() {}

  const GURL& url() const { return url_; }
  PageTransition::Type origin() const { return origin_; }
  base::TimeDelta load_time() const { return load_time_; }
  int session_index() const { return session_index_; }
  NavigationController* controller() const { return controller_; }

 private:
  GURL url_;           
  base::TimeDelta load_time_;               
  int session_index_;  
  PageTransition::Type origin_;       
  NavigationController* controller_;  

  LoadNotificationDetails() {}

  DISALLOW_COPY_AND_ASSIGN(LoadNotificationDetails);
};

#endif  
