// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_THUMBNAIL_GENERATOR_H_
#define CHROME_BROWSER_TAB_CONTENTS_THUMBNAIL_GENERATOR_H_
#pragma once

#include <map>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/linked_ptr.h"
#include "base/timer.h"
#include "content/browser/renderer_host/backing_store.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class GURL;
class Profile;
class RenderWidgetHost;
class SkBitmap;
class TabContents;

namespace history {
class TopSites;
}

class ThumbnailGenerator : NotificationObserver {
 public:
  typedef Callback1<const SkBitmap&>::Type ThumbnailReadyCallback;
  
  
  enum ClipResult {
    
    kSourceIsSmaller,
    
    kWiderThanTall,
    
    kTallerThanWide,
    
    kNotClipped,
  };

  
  enum ThumbnailOptions {
    
    kNoOptions = 0,
    
    kClippedThumbnail = 1 << 0,
  };

  
  ThumbnailGenerator();
  ~ThumbnailGenerator();

  
  
  void StartThumbnailing();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void AskForSnapshot(RenderWidgetHost* renderer,
                      bool prefer_backing_store,
                      ThumbnailReadyCallback* callback,
                      gfx::Size page_size,
                      gfx::Size desired_size);

  
  
  SkBitmap GetThumbnailForRenderer(RenderWidgetHost* renderer) const;

  
  
  // |clip_result| is non-NULL, the result of clipping will be written.
  SkBitmap GetThumbnailForRendererWithOptions(RenderWidgetHost* renderer,
                                              int options,
                                              ClipResult* clip_result) const;

  
  
  void MonitorRenderer(RenderWidgetHost* renderer, bool monitor);

  
  
  
  
  static double CalculateBoringScore(SkBitmap* bitmap);

  
  
  
  
  
  static SkBitmap GetClippedBitmap(const SkBitmap& bitmap,
                                   int desired_width,
                                   int desired_height,
                                   ClipResult* clip_result);

  
  void UpdateThumbnailIfNecessary(TabContents* tab_contents);

  
  static bool ShouldUpdateThumbnail(Profile* profile,
                                    history::TopSites* top_sites,
                                    const GURL& url);

 private:
  virtual void WidgetDidReceivePaintAtSizeAck(
      RenderWidgetHost* widget,
      int tag,
      const gfx::Size& size);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void WidgetHidden(RenderWidgetHost* widget);

  
  
  void TabContentsDisconnected(TabContents* contents);

  NotificationRegistrar registrar_;

  
  struct AsyncRequestInfo;
  typedef std::map<int,
                   linked_ptr<AsyncRequestInfo> > ThumbnailCallbackMap;
  ThumbnailCallbackMap callback_map_;

  DISALLOW_COPY_AND_ASSIGN(ThumbnailGenerator);
};

#endif  
