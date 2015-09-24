// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SITE_INSTANCE_IMPL_H_
#define CONTENT_BROWSER_SITE_INSTANCE_IMPL_H_

#include "content/browser/renderer_host/render_process_host_impl.h"
#include "content/common/content_export.h"
#include "content/public/browser/render_process_host_observer.h"
#include "content/public/browser/site_instance.h"
#include "url/gurl.h"

namespace content {
class RenderProcessHostFactory;

class CONTENT_EXPORT SiteInstanceImpl : public SiteInstance,
                                        public RenderProcessHostObserver {
 public:
  
  virtual int32 GetId() OVERRIDE;
  virtual bool HasProcess() const OVERRIDE;
  virtual RenderProcessHost* GetProcess() OVERRIDE;
  virtual const GURL& GetSiteURL() const OVERRIDE;
  virtual SiteInstance* GetRelatedSiteInstance(const GURL& url) OVERRIDE;
  virtual bool IsRelatedSiteInstance(const SiteInstance* instance) OVERRIDE;
  virtual BrowserContext* GetBrowserContext() const OVERRIDE;

  
  
  
  
  void SetSite(const GURL& url);
  bool HasSite() const;

  
  
  
  bool HasRelatedSiteInstance(const GURL& url);

  
  
  
  bool HasWrongProcessForURL(const GURL& url);

  
  
  
  void increment_active_view_count() { active_view_count_++; }

  
  
  
  void decrement_active_view_count() { active_view_count_--; }

  
  
  
  
  size_t active_view_count() { return active_view_count_; }

  
  
  
  
  static void set_render_process_host_factory(
      const RenderProcessHostFactory* rph_factory);

  
  
  
  
  
  
  static GURL GetEffectiveURL(BrowserContext* browser_context,
                              const GURL& url);

 protected:
  friend class BrowsingInstance;
  friend class SiteInstance;

  
  virtual ~SiteInstanceImpl();

  
  
  
  explicit SiteInstanceImpl(BrowsingInstance* browsing_instance);

 private:
  
  virtual void RenderProcessHostDestroyed(RenderProcessHost* host) OVERRIDE;

  
  void LockToOrigin();

  
  static const RenderProcessHostFactory* g_render_process_host_factory_;

  
  static int32 next_site_instance_id_;

  
  int32 id_;

  
  size_t active_view_count_;

  
  scoped_refptr<BrowsingInstance> browsing_instance_;

  
  
  
  
  RenderProcessHost* process_;

  
  GURL site_;

  
  bool has_site_;

  DISALLOW_COPY_AND_ASSIGN(SiteInstanceImpl);
};

}  

#endif  
