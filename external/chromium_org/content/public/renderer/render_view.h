// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_RENDER_VIEW_H_
#define CONTENT_PUBLIC_RENDERER_RENDER_VIEW_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/public/common/top_controls_state.h"
#include "ipc/ipc_sender.h"
#include "third_party/WebKit/public/web/WebPageVisibilityState.h"
#include "ui/gfx/native_widget_types.h"

struct WebPreferences;

namespace blink {
class WebFrame;
class WebNode;
class WebString;
class WebURLRequest;
class WebView;
struct WebContextMenuData;
}

namespace gfx {
class Size;
}

namespace content {

class RenderFrame;
class RenderViewVisitor;
struct SSLStatus;

class CONTENT_EXPORT RenderView : public IPC::Sender {
 public:
  
  static RenderView* FromWebView(blink::WebView* webview);

  
  static RenderView* FromRoutingID(int routing_id);

  
  
  static void ForEach(RenderViewVisitor* visitor);

  
  virtual RenderFrame* GetMainRenderFrame() = 0;

  
  virtual int GetRoutingID() const = 0;

  
  
  
  
  
  
  virtual int GetPageId() const = 0;

  
  virtual gfx::Size GetSize() const = 0;

  
  virtual WebPreferences& GetWebkitPreferences() = 0;

  
  
  virtual void SetWebkitPreferences(const WebPreferences& preferences) = 0;

  
  virtual blink::WebView* GetWebView() = 0;

  
  virtual blink::WebNode GetFocusedNode() const = 0;

  
  virtual blink::WebNode GetContextMenuNode() const = 0;

  
  
  virtual bool IsEditableNode(const blink::WebNode& node) const = 0;

  
  virtual void EvaluateScript(const base::string16& frame_xpath,
                              const base::string16& jscript,
                              int id,
                              bool notify_result) = 0;

  
  
  virtual bool ShouldDisplayScrollbars(int width, int height) const = 0;

  
  
  virtual int GetEnabledBindings() const = 0;

  
  
  
  virtual bool GetContentStateImmediately() const = 0;

  
  virtual float GetFilteredTimePerFrame() const = 0;

  
  virtual blink::WebPageVisibilityState GetVisibilityState() const = 0;

  
  
  virtual void RunModalAlertDialog(blink::WebFrame* frame,
                                   const blink::WebString& message) = 0;

  
  
  virtual void DidStartLoading() = 0;
  virtual void DidStopLoading() = 0;

  
  
  virtual void Repaint(const gfx::Size& size) = 0;

  
  virtual void SetEditCommandForNextKeyEvent(const std::string& name,
                                             const std::string& value) = 0;
  virtual void ClearEditCommands() = 0;

  
  virtual SSLStatus GetSSLStatusOfFrame(blink::WebFrame* frame) const = 0;

  
  virtual const std::string& GetAcceptLanguages() const = 0;

#if defined(OS_ANDROID)
  virtual void UpdateTopControlsState(TopControlsState constraints,
                                      TopControlsState current,
                                      bool animate) = 0;
#endif

 protected:
  virtual ~RenderView() {}

 private:
  
  friend class RenderViewImpl;
  RenderView() {}
};

}  

#endif  
