// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_TEST_NAVIGATION_OBSERVER_H_
#define CONTENT_PUBLIC_TEST_TEST_NAVIGATION_OBSERVER_H_

#include <set>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/test/test_utils.h"

namespace content {
class WebContents;
struct LoadCommittedDetails;

class TestNavigationObserver {
 public:
  
  
  TestNavigationObserver(WebContents* web_contents,
                         int number_of_navigations);
  
  explicit TestNavigationObserver(WebContents* web_contents);

  virtual ~TestNavigationObserver();

  
  
  void Wait();

  
  void StartWatchingNewWebContents();
  void StopWatchingNewWebContents();

 protected:
  
  void RegisterAsObserver(WebContents* web_contents);

 private:
  class TestWebContentsObserver;

  
  void OnWebContentsCreated(WebContents* web_contents);
  void OnWebContentsDestroyed(TestWebContentsObserver* observer,
                              WebContents* web_contents);
  void OnNavigationEntryCommitted(
      TestWebContentsObserver* observer,
      WebContents* web_contents,
      const LoadCommittedDetails& load_details);
  void OnDidStartLoading(WebContents* web_contents);
  void OnDidStopLoading(WebContents* web_contents);

  
  bool navigation_started_;

  
  int navigations_completed_;

  
  int number_of_navigations_;

  
  scoped_refptr<MessageLoopRunner> message_loop_runner_;

  
  base::Callback<void(WebContents*)> web_contents_created_callback_;

  
  std::set<TestWebContentsObserver*> web_contents_observers_;

  DISALLOW_COPY_AND_ASSIGN(TestNavigationObserver);
};

}  

#endif  
