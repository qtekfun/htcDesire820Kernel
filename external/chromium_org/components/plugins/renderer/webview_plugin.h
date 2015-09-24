// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_PLUGINS_RENDERER_WEBVIEW_PLUGIN_H_
#define COMPONENTS_PLUGINS_RENDERER_WEBVIEW_PLUGIN_H_

#include <list>

#include "base/memory/scoped_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "third_party/WebKit/public/platform/WebCursorInfo.h"
#include "third_party/WebKit/public/platform/WebString.h"
#include "third_party/WebKit/public/platform/WebURLResponse.h"
#include "third_party/WebKit/public/web/WebFrameClient.h"
#include "third_party/WebKit/public/web/WebPlugin.h"
#include "third_party/WebKit/public/web/WebViewClient.h"

struct WebPreferences;

namespace blink {
class WebMouseEvent;
}


class WebViewPlugin : public blink::WebPlugin,
                      public blink::WebViewClient,
                      public blink::WebFrameClient {
 public:
  class Delegate {
   public:
    
    
    
    virtual void BindWebFrame(blink::WebFrame* frame) = 0;

    
    
    virtual void WillDestroyPlugin() = 0;

    
    virtual void ShowContextMenu(const blink::WebMouseEvent&) = 0;
  };

  explicit WebViewPlugin(Delegate* delegate);

  
  
  
  static WebViewPlugin* Create(Delegate* delegate,
                               const WebPreferences& preferences,
                               const std::string& html_data,
                               const GURL& url);

  blink::WebView* web_view() { return web_view_; }

  
  
  
  void ReplayReceivedData(blink::WebPlugin* plugin);

  void RestoreTitleText();

  
  virtual blink::WebPluginContainer* container() const;
  virtual bool initialize(blink::WebPluginContainer*);
  virtual void destroy();

  virtual NPObject* scriptableObject();
  virtual struct _NPP* pluginNPP();

  virtual bool getFormValue(blink::WebString& value);

  virtual void paint(blink::WebCanvas* canvas, const blink::WebRect& rect);

  
  virtual void updateGeometry(
      const blink::WebRect& frame_rect,
      const blink::WebRect& clip_rect,
      const blink::WebVector<blink::WebRect>& cut_out_rects,
      bool is_visible);

  virtual void updateFocus(bool) {}
  virtual void updateVisibility(bool) {}

  virtual bool acceptsInputEvents();
  virtual bool handleInputEvent(const blink::WebInputEvent& event,
                                blink::WebCursorInfo& cursor_info);

  virtual void didReceiveResponse(const blink::WebURLResponse& response);
  virtual void didReceiveData(const char* data, int data_length);
  virtual void didFinishLoading();
  virtual void didFailLoading(const blink::WebURLError& error);

  
  virtual void didFinishLoadingFrameRequest(const blink::WebURL& url,
                                            void* notifyData) {}
  virtual void didFailLoadingFrameRequest(const blink::WebURL& url,
                                          void* notify_data,
                                          const blink::WebURLError& error) {}

  
  virtual bool acceptsLoadDrops();

  virtual void setToolTipText(const blink::WebString&,
                              blink::WebTextDirection);

  virtual void startDragging(blink::WebFrame* frame,
                             const blink::WebDragData& drag_data,
                             blink::WebDragOperationsMask mask,
                             const blink::WebImage& image,
                             const blink::WebPoint& point);

  
  virtual void didInvalidateRect(const blink::WebRect&);
  virtual void didChangeCursor(const blink::WebCursorInfo& cursor);

  
  virtual void didClearWindowObject(blink::WebFrame* frame);

  
  
  
  
  virtual void didReceiveResponse(blink::WebFrame* frame,
                                  unsigned identifier,
                                  const blink::WebURLResponse& response);

 private:
  friend class base::DeleteHelper<WebViewPlugin>;
  virtual ~WebViewPlugin();

  Delegate* delegate_;
  
  blink::WebCursorInfo current_cursor_;
  
  blink::WebPluginContainer* container_;
  
  blink::WebView* web_view_;
  gfx::Rect rect_;

  blink::WebURLResponse response_;
  std::list<std::string> data_;
  bool finished_loading_;
  scoped_ptr<blink::WebURLError> error_;
  blink::WebString old_title_;
};

#endif  
