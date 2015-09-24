// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_CHROME_RENDER_VIEW_OBSERVER_H_
#define CHROME_RENDERER_CHROME_RENDER_VIEW_OBSERVER_H_

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "content/public/common/top_controls_state.h"
#include "content/public/renderer/render_view_observer.h"
#include "ui/gfx/size.h"
#include "url/gurl.h"

class ChromeRenderProcessObserver;
class ContentSettingsObserver;
class ExternalHostBindings;
class SkBitmap;
class TranslateHelper;
class WebViewColorOverlay;
class WebViewAnimatingOverlay;

namespace blink {
class WebView;
struct WebWindowFeatures;
}

namespace safe_browsing {
class PhishingClassifierDelegate;
}

class ChromeRenderViewObserver : public content::RenderViewObserver {
 public:
  
  ChromeRenderViewObserver(
      content::RenderView* render_view,
      ChromeRenderProcessObserver* chrome_render_process_observer);
  virtual ~ChromeRenderViewObserver();

 private:
  
  
  struct WebUIJavaScript {
    base::string16 frame_xpath;
    base::string16 jscript;
    int id;
    bool notify_result;
  };

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void DidStartLoading() OVERRIDE;
  virtual void DidStopLoading() OVERRIDE;
  virtual void DidCommitProvisionalLoad(blink::WebFrame* frame,
                                        bool is_new_navigation) OVERRIDE;
  virtual void DidClearWindowObject(blink::WebFrame* frame) OVERRIDE;
  virtual void DetailedConsoleMessageAdded(const base::string16& message,
                                           const base::string16& source,
                                           const base::string16& stack_trace,
                                           int32 line_number,
                                           int32 severity_level) OVERRIDE;
  virtual void Navigate(const GURL& url) OVERRIDE;

  void OnWebUIJavaScript(const base::string16& frame_xpath,
                         const base::string16& jscript,
                         int id,
                         bool notify_result);
  void OnHandleMessageFromExternalHost(const std::string& message,
                                       const std::string& origin,
                                       const std::string& target);
  void OnJavaScriptStressTestControl(int cmd, int param);
  void OnSetClientSidePhishingDetection(bool enable_phishing_detection);
  void OnSetVisuallyDeemphasized(bool deemphasized);
  void OnRequestThumbnailForContextNode(int thumbnail_min_area_pixels,
                                        gfx::Size thumbnail_max_size_pixels);
  void OnGetFPS();
#if defined(OS_ANDROID)
  void OnUpdateTopControlsState(content::TopControlsState constraints,
                                content::TopControlsState current,
                                bool animate);
  void OnRetrieveWebappInformation(const GURL& expected_url);
#endif
  void OnSetWindowFeatures(const blink::WebWindowFeatures& window_features);

  void CapturePageInfoLater(int page_id,
                            bool preliminary_capture,
                            base::TimeDelta delay);

  
  
  void CapturePageInfo(int page_id, bool preliminary_capture);

  
  
  void CaptureText(blink::WebFrame* frame, base::string16* contents);

  ExternalHostBindings* GetExternalHostBindings();

  
  bool IsStrictSecurityHost(const std::string& host);

  
  bool HasRefreshMetaTag(blink::WebFrame* frame);

  
  scoped_ptr<WebUIJavaScript> webui_javascript_;

  
  ChromeRenderProcessObserver* chrome_render_process_observer_;

  
  TranslateHelper* translate_helper_;
  safe_browsing::PhishingClassifierDelegate* phishing_classifier_;

  
  
  int32 last_indexed_page_id_;
  
  
  
  GURL last_indexed_url_;

  
  scoped_ptr<ExternalHostBindings> external_host_bindings_;

  
  scoped_ptr<WebViewColorOverlay> dimmed_color_overlay_;

  
  base::Timer capture_timer_;

  DISALLOW_COPY_AND_ASSIGN(ChromeRenderViewObserver);
};

#endif  
