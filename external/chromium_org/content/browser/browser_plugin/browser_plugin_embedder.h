// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_BROWSER_PLUGIN_BROWSER_PLUGIN_EMBEDDER_H_
#define CONTENT_BROWSER_BROWSER_PLUGIN_BROWSER_PLUGIN_EMBEDDER_H_

#include <map>

#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"

struct BrowserPluginHostMsg_Attach_Params;
struct BrowserPluginHostMsg_ResizeGuest_Params;

namespace gfx {
class Point;
}

namespace content {

class BrowserPluginGuest;
class BrowserPluginGuestManager;
class BrowserPluginHostFactory;
class RenderWidgetHostImpl;
class WebContentsImpl;
struct NativeWebKeyboardEvent;

class CONTENT_EXPORT BrowserPluginEmbedder : public WebContentsObserver {
 public:
  virtual ~BrowserPluginEmbedder();

  static BrowserPluginEmbedder* Create(WebContentsImpl* web_contents);

  
  
  
  void GetRenderViewHostAtPosition(
      int x,
      int y,
      const WebContents::GetRenderViewHostCallback& callback);

  
  void DidSendScreenRects();

  
  
  
  
  bool HandleKeyboardEvent(const NativeWebKeyboardEvent& event);

  
  
  static void set_factory_for_testing(BrowserPluginHostFactory* factory) {
    factory_ = factory;
  }

  
  void SetZoomLevel(double level);

  
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void DragSourceEndedAt(int client_x, int client_y, int screen_x,
      int screen_y, blink::WebDragOperation operation);

  void DragSourceMovedTo(int client_x, int client_y,
                         int screen_x, int screen_y);

  void OnUpdateDragCursor(bool* handled);

  void DragEnteredGuest(BrowserPluginGuest* guest);

  void DragLeftGuest(BrowserPluginGuest* guest);

  void StartDrag(BrowserPluginGuest* guest);

  void StopDrag(BrowserPluginGuest* guest);

  void SystemDragEnded();

 private:
  friend class TestBrowserPluginEmbedder;

  BrowserPluginEmbedder(WebContentsImpl* web_contents);

  void CleanUp();

  BrowserPluginGuestManager* GetBrowserPluginGuestManager();

  bool DidSendScreenRectsCallback(BrowserPluginGuest* guest);

  bool SetZoomLevelCallback(double level, BrowserPluginGuest* guest);

  bool UnlockMouseIfNecessaryCallback(const NativeWebKeyboardEvent& event,
                                      BrowserPluginGuest* guest);

  

  void OnAllocateInstanceID(int request_id);
  void OnAttach(int instance_id,
                const BrowserPluginHostMsg_Attach_Params& params,
                const base::DictionaryValue& extra_params);
  void OnPluginAtPositionResponse(int instance_id,
                                  int request_id,
                                  const gfx::Point& position);

  
  static BrowserPluginHostFactory* factory_;

  
  
  
  typedef std::map<int, WebContents::GetRenderViewHostCallback>
      GetRenderViewHostCallbackMap;
  GetRenderViewHostCallbackMap pending_get_render_view_callbacks_;
  
  int next_get_render_view_request_id_;

  
  
  
  
  
  base::WeakPtr<BrowserPluginGuest> guest_dragging_over_;

  
  
  base::WeakPtr<BrowserPluginGuest> guest_started_drag_;

  DISALLOW_COPY_AND_ASSIGN(BrowserPluginEmbedder);
};

}  

#endif  
