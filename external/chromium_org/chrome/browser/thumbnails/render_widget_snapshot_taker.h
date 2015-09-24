// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THUMBNAILS_RENDER_WIDGET_SNAPSHOT_TAKER_H_
#define CHROME_BROWSER_THUMBNAILS_RENDER_WIDGET_SNAPSHOT_TAKER_H_

#include <map>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class SkBitmap;

namespace content {
class RenderWidgetHost;
}

namespace gfx {
class Size;
}

class RenderWidgetSnapshotTaker : public content::NotificationObserver {
 public:
  typedef base::Callback<void(const SkBitmap&)> SnapshotReadyCallback;

  RenderWidgetSnapshotTaker();
  virtual ~RenderWidgetSnapshotTaker();

  
  
  
  
  
  
  
  
  
  
  
  
  
  void AskForSnapshot(content::RenderWidgetHost* renderer,
                      const SnapshotReadyCallback& callback,
                      gfx::Size page_size,
                      gfx::Size desired_size);

  
  void CancelSnapshot(content::RenderWidgetHost* renderer);

 private:
  FRIEND_TEST_ALL_PREFIXES(RenderWidgetSnapshotTakerTest,
                           WidgetDidReceivePaintAtSizeAck);
  FRIEND_TEST_ALL_PREFIXES(RenderWidgetSnapshotTakerTest,
                           WidgetDidReceivePaintAtSizeAckFail);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void MonitorRenderer(content::RenderWidgetHost* renderer, bool monitor);

  void WidgetDidReceivePaintAtSizeAck(content::RenderWidgetHost* widget,
                                      int tag,
                                      const gfx::Size& size);

  content::NotificationRegistrar registrar_;

  
  struct AsyncRequestInfo;
  typedef std::map<int,
                   linked_ptr<AsyncRequestInfo> > SnapshotCallbackMap;
  SnapshotCallbackMap callback_map_;

  
  
  std::map<content::RenderWidgetHost*, int> host_monitor_counts_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidgetSnapshotTaker);
};

#endif  
