// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_RENDER_VIEW_OBSERVER_H_
#define CONTENT_PUBLIC_RENDERER_RENDER_VIEW_OBSERVER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/common/content_export.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "third_party/WebKit/public/platform/WebVector.h"
#include "third_party/WebKit/public/web/WebIconURL.h"

class GURL;

namespace ppapi {
namespace host {
class PpapiHost;
}
}

namespace blink {
class WebDataSource;
class WebFrame;
class WebFormElement;
class WebGestureEvent;
class WebMediaPlayerClient;
class WebMouseEvent;
class WebNode;
class WebTouchEvent;
class WebURL;
struct WebContextMenuData;
struct WebURLError;
}

namespace content {

class RendererPpapiHost;
class RenderView;
class RenderViewImpl;

class CONTENT_EXPORT RenderViewObserver : public IPC::Listener,
                                          public IPC::Sender {
 public:
  
  
  virtual void OnDestruct();

  
  virtual void DidStartLoading() {}
  virtual void DidStopLoading() {}
  virtual void DidFinishDocumentLoad(blink::WebFrame* frame) {}
  virtual void DidFailLoad(blink::WebFrame* frame,
                           const blink::WebURLError& error) {}
  virtual void DidFinishLoad(blink::WebFrame* frame) {}
  virtual void DidStartProvisionalLoad(blink::WebFrame* frame) {}
  virtual void DidFailProvisionalLoad(blink::WebFrame* frame,
                                      const blink::WebURLError& error) {}
  virtual void DidCommitProvisionalLoad(blink::WebFrame* frame,
                                        bool is_new_navigation) {}
  virtual void DidClearWindowObject(blink::WebFrame* frame) {}
  virtual void DidCreateDocumentElement(blink::WebFrame* frame) {}
  virtual void FrameCreated(blink::WebFrame* parent,
                            blink::WebFrame* frame) {}
  virtual void FrameDetached(blink::WebFrame* frame) {}
  virtual void FrameWillClose(blink::WebFrame* frame) {}
  virtual void DidMatchCSS(
      blink::WebFrame* frame,
      const blink::WebVector<blink::WebString>& newly_matching_selectors,
      const blink::WebVector<blink::WebString>& stopped_matching_selectors) {}
  virtual void WillSendSubmitEvent(blink::WebFrame* frame,
                                   const blink::WebFormElement& form) {}
  virtual void WillSubmitForm(blink::WebFrame* frame,
                              const blink::WebFormElement& form) {}
  virtual void DidCreateDataSource(blink::WebFrame* frame,
                                   blink::WebDataSource* ds) {}
  virtual void PrintPage(blink::WebFrame* frame, bool user_initiated) {}
  virtual void FocusedNodeChanged(const blink::WebNode& node) {}
  virtual void WillCreateMediaPlayer(blink::WebFrame* frame,
                                     blink::WebMediaPlayerClient* client) {}
  virtual void ZoomLevelChanged() {};
  virtual void DidChangeScrollOffset(blink::WebFrame* frame) {}
  virtual void DraggableRegionsChanged(blink::WebFrame* frame) {}
  virtual void DidRequestShowContextMenu(
      blink::WebFrame* frame,
      const blink::WebContextMenuData& data) {}
  virtual void DidCommitCompositorFrame() {}
  virtual void DidUpdateLayout() {}

  
  virtual void DidHandleMouseEvent(const blink::WebMouseEvent& event) {}
  virtual void DidHandleTouchEvent(const blink::WebTouchEvent& event) {}

  
  
  
  
  
  
  virtual void DetailedConsoleMessageAdded(const base::string16& message,
                                           const base::string16& source,
                                           const base::string16& stack_trace,
                                           int32 line_number,
                                           int32 severity_level) {}

  
  virtual void Navigate(const GURL& url) {}
  virtual void ClosePage() {}
  virtual void OrientationChangeEvent(int orientation) {}

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  RenderView* render_view() const;
  int routing_id() const { return routing_id_; }

 protected:
  explicit RenderViewObserver(RenderView* render_view);
  virtual ~RenderViewObserver();

 private:
  friend class RenderViewImpl;

  
  
  void RenderViewGone();

  RenderView* render_view_;
  
  int routing_id_;

  DISALLOW_COPY_AND_ASSIGN(RenderViewObserver);
};

}  

#endif  
