// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_LINK_MANAGER_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_LINK_MANAGER_H_

#include <list>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/time/time.h"
#include "chrome/browser/prerender/prerender_handle.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "url/gurl.h"

class Profile;

namespace content {
struct Referrer;
}

namespace gfx {
class Size;
}

FORWARD_DECLARE_TEST(WebViewTest, NoPrerenderer);

namespace prerender {

class PrerenderHandle;
class PrerenderManager;

class PrerenderLinkManager : public BrowserContextKeyedService,
                             public PrerenderHandle::Observer {
 public:
  explicit PrerenderLinkManager(PrerenderManager* manager);
  virtual ~PrerenderLinkManager();

  
  
  
  void OnAddPrerender(int child_id,
                      int prerender_id,
                      const GURL& url,
                      const content::Referrer& referrer,
                      const gfx::Size& size,
                      int render_view_route_id);

  
  
  void OnCancelPrerender(int child_id, int prerender_id);

  
  
  
  
  void OnAbandonPrerender(int child_id, int prerender_id);

  
  
  void OnChannelClosing(int child_id);

 private:
  friend class PrerenderBrowserTest;
  friend class PrerenderTest;
  
  FRIEND_TEST_ALL_PREFIXES(::WebViewTest, NoPrerenderer);

  struct LinkPrerender {
    LinkPrerender(int launcher_child_id,
                  int prerender_id,
                  const GURL& url,
                  const content::Referrer& referrer,
                  const gfx::Size& size,
                  int render_view_route_id,
                  base::TimeTicks creation_time);
    ~LinkPrerender();

    
    int launcher_child_id;
    int prerender_id;
    GURL url;
    content::Referrer referrer;
    gfx::Size size;
    int render_view_route_id;

    
    base::TimeTicks creation_time;

    
    
    PrerenderHandle* handle;

    
    
    bool is_match_complete_replacement;

    
    bool has_been_abandoned;
  };

  bool IsEmpty() const;

  
  size_t CountRunningPrerenders() const;

  
  
  void StartPrerenders();

  LinkPrerender* FindByLauncherChildIdAndPrerenderId(int child_id,
                                                     int prerender_id);

  LinkPrerender* FindByPrerenderHandle(PrerenderHandle* prerender_handle);

  
  
  void RemovePrerender(LinkPrerender* prerender);

  
  
  void CancelPrerender(LinkPrerender* prerender);

  
  virtual void Shutdown() OVERRIDE;

  
  virtual void OnPrerenderStart(PrerenderHandle* prerender_handle) OVERRIDE;
  virtual void OnPrerenderStopLoading(PrerenderHandle* prerender_handle)
      OVERRIDE;
  virtual void OnPrerenderStop(PrerenderHandle* prerender_handle) OVERRIDE;
  virtual void OnPrerenderCreatedMatchCompleteReplacement(
      PrerenderHandle* handle) OVERRIDE;

  bool has_shutdown_;

  PrerenderManager* manager_;

  
  
  
  std::list<LinkPrerender> prerenders_;

  DISALLOW_COPY_AND_ASSIGN(PrerenderLinkManager);
};

}  

#endif  

