// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_REMOTING_PAGE_LOAD_NOTIFICATION_OBSERVER_H_
#define CHROME_TEST_REMOTING_PAGE_LOAD_NOTIFICATION_OBSERVER_H_

#include "content/public/test/test_utils.h"
#include "url/gurl.h"

namespace remoting {

class PageLoadNotificationObserver
    : public content::WindowedNotificationObserver {
 public:
  explicit PageLoadNotificationObserver(const GURL& target);

  virtual ~PageLoadNotificationObserver();

 private:
  bool IsTargetLoaded();
  GURL target_;
  DISALLOW_COPY_AND_ASSIGN(PageLoadNotificationObserver);
};

}  

#endif  
