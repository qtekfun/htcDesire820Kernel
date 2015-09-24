// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_WEB_CONTENTS_TRACKER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_WEB_CONTENTS_TRACKER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "content/public/browser/web_contents_observer.h"

namespace base {
class MessageLoopProxy;
}

namespace content {

class CONTENT_EXPORT WebContentsTracker
    : public base::RefCountedThreadSafe<WebContentsTracker>,
      public WebContentsObserver {
 public:
  WebContentsTracker();

  
  
  
  
  typedef base::Callback<void(int render_process_id, int render_view_id)>
      ChangeCallback;

  
  
  
  virtual void Start(int render_process_id, int render_view_id,
                     const ChangeCallback& callback);

  
  
  virtual void Stop();

 protected:
  friend class base::RefCountedThreadSafe<WebContentsTracker>;
  virtual ~WebContentsTracker();

 private:
  
  
  void OnWebContentsChangeEvent();

  
  
  void MaybeDoCallback(int render_process_id, int render_view_id);

  
  
  void LookUpAndObserveWebContents(int render_process_id,
                                   int render_view_id);

  
  virtual void RenderViewReady() OVERRIDE;
  virtual void AboutToNavigateRenderView(RenderViewHost* render_view_host)
      OVERRIDE;
  virtual void DidNavigateMainFrame(const LoadCommittedDetails& details,
                                    const FrameNavigateParams& params) OVERRIDE;
  virtual void WebContentsDestroyed(WebContents* web_contents) OVERRIDE;

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  ChangeCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(WebContentsTracker);
};

}  

#endif  
