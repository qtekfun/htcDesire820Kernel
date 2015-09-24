// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THUMBNAILS_THUMBNAIL_TAB_HELPER_H_
#define CHROME_BROWSER_THUMBNAILS_THUMBNAIL_TAB_HELPER_H_

#include "base/basictypes.h"
#include "chrome/browser/thumbnails/thumbnailing_context.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class RenderViewHost;
class RenderWidgetHost;
}

class ThumbnailTabHelper
    : public content::NotificationObserver,
      public content::WebContentsObserver,
      public content::WebContentsUserData<ThumbnailTabHelper> {
 public:
  virtual ~ThumbnailTabHelper();

  
  
  
  void set_enabled(bool enabled) { enabled_ = enabled; }

 private:
  explicit ThumbnailTabHelper(content::WebContents* contents);
  friend class content::WebContentsUserData<ThumbnailTabHelper>;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void RenderViewDeleted(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidStartLoading(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void NavigationStopped() OVERRIDE;

  
  void UpdateThumbnailIfNecessary(content::WebContents* web_contents);

  
  void RenderViewHostCreated(content::RenderViewHost* renderer);

  
  void WidgetHidden(content::RenderWidgetHost* widget);

  bool enabled_;

  content::NotificationRegistrar registrar_;

  bool load_interrupted_;

  DISALLOW_COPY_AND_ASSIGN(ThumbnailTabHelper);
};

#endif  
