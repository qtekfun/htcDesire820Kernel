// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_PRERENDER_PRERENDER_DISPATCHER_H_
#define CHROME_RENDERER_PRERENDER_PRERENDER_DISPATCHER_H_

#include <map>
#include <set>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/renderer/render_process_observer.h"
#include "third_party/WebKit/public/platform/WebPrerender.h"
#include "third_party/WebKit/public/platform/WebPrerenderingSupport.h"

class GURL;

namespace prerender {

class PrerenderDispatcher : public content::RenderProcessObserver,
                            public blink::WebPrerenderingSupport {
 public:
  PrerenderDispatcher();
  virtual ~PrerenderDispatcher();

  bool IsPrerenderURL(const GURL& url) const;

 private:
  friend class PrerenderDispatcherTest;

  
  void OnPrerenderStart(int prerender_id);
  void OnPrerenderStopLoading(int prerender_id);
  void OnPrerenderAddAlias(const GURL& alias);
  void OnPrerenderRemoveAliases(const std::vector<GURL>& aliases);
  void OnPrerenderStop(int prerender_id);

  
  virtual bool OnControlMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual void add(const blink::WebPrerender& prerender) OVERRIDE;
  virtual void cancel(const blink::WebPrerender& prerender) OVERRIDE;
  virtual void abandon(const blink::WebPrerender& prerender) OVERRIDE;

  
  std::map<int, blink::WebPrerender> prerenders_;

  
  
  std::multiset<GURL> running_prerender_urls_;
};

}  

#endif  
