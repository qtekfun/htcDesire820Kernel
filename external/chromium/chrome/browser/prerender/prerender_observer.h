// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_OBSERVER_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_OBSERVER_H_
#pragma once

#include <list>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "content/browser/tab_contents/tab_contents_observer.h"
#include "googleurl/src/gurl.h"

class PrerenderContents;
class Profile;
class TabContents;

namespace prerender {

class PrerenderManager;

class PrerenderObserver : public TabContentsObserver {
 public:
  explicit PrerenderObserver(TabContents* tab_contents);
  virtual ~PrerenderObserver();

  
  virtual void ProvisionalChangeToMainFrameUrl(const GURL& url);
  virtual bool OnMessageReceived(const IPC::Message& message);

  
  void OnDidStartProvisionalLoadForFrame(int64 frame_id,
                                         bool main_frame,
                                         const GURL& url);

  virtual void DidStopLoading();

 private:
  
  PrerenderManager* MaybeGetPrerenderManager();

  
  
  
  bool MaybeUsePreloadedPage(const GURL& url);

  
  
  base::TimeTicks pplt_load_start_;

  DISALLOW_COPY_AND_ASSIGN(PrerenderObserver);
};

}  

#endif  
