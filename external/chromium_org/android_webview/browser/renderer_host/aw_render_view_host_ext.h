// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_RENDER_HOST_RENDER_VIEW_HOST_EXT_H_
#define ANDROID_WEBVIEW_BROWSER_RENDER_HOST_RENDER_VIEW_HOST_EXT_H_

#include "content/public/browser/web_contents_observer.h"

#include "android_webview/common/aw_hit_test_data.h"
#include "base/callback_forward.h"
#include "base/threading/non_thread_safe.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/size.h"

class GURL;

namespace content {
struct FrameNavigateParams;
struct LoadCommittedDetails;
}  

namespace android_webview {

class AwRenderViewHostExtClient {
 public:
  
  virtual void OnWebLayoutPageScaleFactorChanged(float page_scale_factor) = 0;
  virtual void OnWebLayoutContentsSizeChanged(
      const gfx::Size& contents_size) = 0;

 protected:
  virtual ~AwRenderViewHostExtClient() {}
};

class AwRenderViewHostExt : public content::WebContentsObserver,
                            public base::NonThreadSafe {
 public:

  
  
  AwRenderViewHostExt(
      AwRenderViewHostExtClient* client, content::WebContents* contents);
  virtual ~AwRenderViewHostExt();

  
  typedef base::Callback<void(bool)> DocumentHasImagesResult;
  void DocumentHasImages(DocumentHasImagesResult result);

  
  void ClearCache();

  
  
  
  void RequestNewHitTestDataAt(int view_x, int view_y);

  
  bool HasNewHitTestData() const;
  void MarkHitTestDataRead();

  
  
  const AwHitTestData& GetLastHitTestData() const;

  
  
  void SetTextZoomFactor(float factor);

  void SetFixedLayoutSize(const gfx::Size& size);

  void ResetScrollAndScaleState();

  
  
  void SetInitialPageScale(double page_scale_factor);
  void SetBackgroundColor(SkColor c);
  void SetJsOnlineProperty(bool network_up);

 private:
  
  virtual void RenderViewCreated(content::RenderViewHost* view_host) OVERRIDE;
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;
  virtual void DidNavigateAnyFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void OnDocumentHasImagesResponse(int msg_id, bool has_images);
  void OnUpdateHitTestData(const AwHitTestData& hit_test_data);
  void OnPageScaleFactorChanged(float page_scale_factor);
  void OnContentsSizeChanged(const gfx::Size& contents_size);

  bool IsRenderViewReady() const;

  AwRenderViewHostExtClient* client_;

  SkColor background_color_;

  std::map<int, DocumentHasImagesResult> pending_document_has_images_requests_;

  
  
  
  AwHitTestData last_hit_test_data_;

  bool has_new_hit_test_data_;

  DISALLOW_COPY_AND_ASSIGN(AwRenderViewHostExt);
};

}  

#endif  
