// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_FIND_IN_PAGE_OBSERVER_H_
#define CHROME_TEST_BASE_FIND_IN_PAGE_OBSERVER_H_

#include "base/memory/ref_counted.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/gfx/rect.h"

namespace content {
class MessageLoopRunner;
class WebContents;
}

namespace ui_test_utils {


class FindInPageNotificationObserver : public content::NotificationObserver {
 public:
  explicit FindInPageNotificationObserver(content::WebContents* parent_tab);
  virtual ~FindInPageNotificationObserver();

  void Wait();

  int active_match_ordinal() const { return active_match_ordinal_; }
  int number_of_matches() const { return number_of_matches_; }
  gfx::Rect selection_rect() const { return selection_rect_; }

 private:
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;
  
  
  int active_match_ordinal_;
  int number_of_matches_;
  gfx::Rect selection_rect_;
  
  
  int current_find_request_id_;
  scoped_refptr<content::MessageLoopRunner> message_loop_runner_;

  bool seen_; 
  bool running_; 

  DISALLOW_COPY_AND_ASSIGN(FindInPageNotificationObserver);
};

}  

#endif  
