// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_SITE_INSTANCE_H_
#define CONTENT_PUBLIC_BROWSER_SITE_INSTANCE_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "url/gurl.h"

namespace content {
class BrowserContext;
class BrowsingInstance;
class RenderProcessHost;

class CONTENT_EXPORT SiteInstance : public base::RefCounted<SiteInstance> {
 public:
  
  virtual int32 GetId() = 0;

  
  
  
  virtual bool HasProcess() const = 0;

  
  
  
  
  
  
  
  
  
  
  virtual content::RenderProcessHost* GetProcess() = 0;

  
  
  virtual content::BrowserContext* GetBrowserContext() const = 0;

  
  
  virtual const GURL& GetSiteURL() const = 0;

  
  
  
  
  
  
  
  
  
  virtual SiteInstance* GetRelatedSiteInstance(const GURL& url) = 0;

  
  
  
  virtual bool IsRelatedSiteInstance(const SiteInstance* instance) = 0;

  
  
  
  
  
  
  
  
  
  static SiteInstance* Create(content::BrowserContext* browser_context);

  
  
  
  
  static SiteInstance* CreateForURL(
      content::BrowserContext* browser_context, const GURL& url);

  
  
  
  
  
  
  
  static bool IsSameWebSite(content::BrowserContext* browser_context,
                            const GURL& url1, const GURL& url2);

  
  
  static GURL GetSiteForURL(BrowserContext* context, const GURL& url);

 protected:
  friend class base::RefCounted<SiteInstance>;

  SiteInstance() {}
  virtual ~SiteInstance() {}
};

}  

#endif  
