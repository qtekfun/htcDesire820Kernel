// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_VIEW_HOST_DELEGATE_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_VIEW_HOST_DELEGATE_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/i18n/rtl.h"
#include "base/process/kill.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/public/common/javascript_message_type.h"
#include "content/public/common/media_stream_request.h"
#include "content/public/common/page_transition_types.h"
#include "net/base/load_states.h"
#include "third_party/WebKit/public/web/WebPopupType.h"
#include "ui/base/window_open_disposition.h"

class GURL;
class SkBitmap;
class WebKeyboardEvent;
struct ViewHostMsg_CreateWindow_Params;
struct ViewHostMsg_DidFailProvisionalLoadWithError_Params;
struct ViewHostMsg_FrameNavigate_Params;
struct ViewMsg_PostMessage_Params;
struct WebPreferences;

namespace base {
class ListValue;
class TimeTicks;
}

namespace IPC {
class Message;
}

namespace gfx {
class Point;
class Rect;
class Size;
}

namespace content {

class BrowserContext;
class FrameTree;
class PageState;
class RenderViewHost;
class RenderViewHostDelegateView;
class SessionStorageNamespace;
class SiteInstance;
class WebContents;
class WebContentsImpl;
struct ContextMenuParams;
struct FileChooserParams;
struct GlobalRequestID;
struct NativeWebKeyboardEvent;
struct Referrer;
struct RendererPreferences;
struct ResourceRedirectDetails;
struct ResourceRequestDetails;

class CONTENT_EXPORT RenderViewHostDelegate {
 public:
  
  
  

  class CONTENT_EXPORT RendererManagement {
   public:
    
    
    
    
    
    
    virtual void ShouldClosePage(
        bool for_cross_site_transition,
        bool proceed,
        const base::TimeTicks& proceed_time) = 0;

    
    
    
    virtual void OnCrossSiteResponse(
        RenderViewHost* pending_render_view_host,
        const GlobalRequestID& global_request_id,
        bool is_transfer,
        const std::vector<GURL>& transfer_url_chain,
        const Referrer& referrer,
        PageTransition page_transition,
        int64 frame_id,
        bool should_replace_current_entry) = 0;

   protected:
    virtual ~RendererManagement() {}
  };

  

  
  
  virtual RenderViewHostDelegateView* GetDelegateView();
  virtual RendererManagement* GetRendererManagementDelegate();

  
  virtual bool OnMessageReceived(RenderViewHost* render_view_host,
                                 const IPC::Message& message);

  
  virtual const GURL& GetURL() const;

  
  
  
  virtual WebContents* GetAsWebContents();

  
  
  virtual gfx::Rect GetRootWindowResizerRect() const = 0;

  
  
  
  virtual void RenderViewCreated(RenderViewHost* render_view_host) {}

  
  virtual void RenderViewReady(RenderViewHost* render_view_host) {}

  
  virtual void RenderViewTerminated(RenderViewHost* render_view_host,
                                    base::TerminationStatus status,
                                    int error_code) {}

  
  
  virtual void RenderViewDeleted(RenderViewHost* render_view_host) {}

  
  
  
  
  
  virtual void DidRedirectProvisionalLoad(
      RenderViewHost* render_view_host,
      int32 page_id,
      const GURL& source_url,
      const GURL& target_url) {}

  
  virtual void DidFailProvisionalLoadWithError(
      RenderViewHost* render_view_host,
      const ViewHostMsg_DidFailProvisionalLoadWithError_Params& params) {}

  
  virtual void DidGetResourceResponseStart(
      const ResourceRequestDetails& details) {}

  
  virtual void DidGetRedirectForResourceRequest(
      const ResourceRedirectDetails& details) {}

  
  virtual void DidNavigate(RenderViewHost* render_view_host,
                           const ViewHostMsg_FrameNavigate_Params& params) {}

  
  virtual void UpdateState(RenderViewHost* render_view_host,
                           int32 page_id,
                           const PageState& state) {}

  
  virtual void UpdateTitle(RenderViewHost* render_view_host,
                           int32 page_id,
                           const base::string16& title,
                           base::i18n::TextDirection title_direction) {}

  
  virtual void UpdateEncoding(RenderViewHost* render_view_host,
                              const std::string& encoding) {}

  
  virtual void UpdateTargetURL(int32 page_id, const GURL& url) {}

  
  virtual void Close(RenderViewHost* render_view_host) {}

  
  virtual void SwappedOut(RenderViewHost* render_view_host) {}

  
  virtual void RequestMove(const gfx::Rect& new_bounds) {}

  
  
  virtual void DidStartLoading(RenderViewHost* render_view_host) {}

  
  
  virtual void DidStopLoading(RenderViewHost* render_view_host) {}

  
  virtual void DidCancelLoading() {}

  
  
  
  virtual void DidChangeLoadProgress(double progress) {}

  
  
  virtual void DidDisownOpener(RenderViewHost* rvh) {}

  
  
  
  virtual void DidAccessInitialDocument() {}

  
  
  virtual void DocumentAvailableInMainFrame(RenderViewHost* render_view_host) {}

  
  virtual void DocumentOnLoadCompletedInMainFrame(
      RenderViewHost* render_view_host,
      int32 page_id) {}

  
  virtual void RequestOpenURL(RenderViewHost* rvh,
                              const GURL& url,
                              const Referrer& referrer,
                              WindowOpenDisposition disposition,
                              int64 source_frame_id,
                              bool is_redirect,
                              bool user_gesture) {}

  
  
  
  virtual void RequestTransferURL(
      const GURL& url,
      const std::vector<GURL>& redirect_chain,
      const Referrer& referrer,
      PageTransition page_transition,
      WindowOpenDisposition disposition,
      int64 source_frame_id,
      const GlobalRequestID& old_request_id,
      bool is_redirect,
      bool user_gesture) {}

  
  virtual void RouteCloseEvent(RenderViewHost* rvh) {}

  
  virtual void RouteMessageEvent(
      RenderViewHost* rvh,
      const ViewMsg_PostMessage_Params& params) {}

  
  virtual void RunJavaScriptMessage(RenderViewHost* rvh,
                                    const base::string16& message,
                                    const base::string16& default_prompt,
                                    const GURL& frame_url,
                                    JavaScriptMessageType type,
                                    IPC::Message* reply_msg,
                                    bool* did_suppress_message) {}

  virtual void RunBeforeUnloadConfirm(RenderViewHost* rvh,
                                      const base::string16& message,
                                      bool is_reload,
                                      IPC::Message* reply_msg) {}

  
  virtual bool AddMessageToConsole(int32 level,
                                   const base::string16& message,
                                   int32 line_no,
                                   const base::string16& source_id);

  
  
  virtual RendererPreferences GetRendererPrefs(
      BrowserContext* browser_context) const = 0;

  
  
  virtual WebPreferences GetWebkitPrefs();

  
  
  
  virtual void OnUserGesture() {}

  
  
  
  
  virtual void OnIgnoredUIEvent() {}

  
  
  virtual void RendererUnresponsive(RenderViewHost* render_view_host,
                                    bool is_during_before_unload,
                                    bool is_during_unload) {}

  
  
  
  virtual void RendererResponsive(RenderViewHost* render_view_host) {}

  
  virtual void LoadStateChanged(const GURL& url,
                                const net::LoadStateWithParam& load_state,
                                uint64 upload_position,
                                uint64 upload_size) {}

  
  virtual void WorkerCrashed() {}

  
  
  virtual void Activate() {}
  virtual void Deactivate() {}

  
  virtual void LostCapture() {}

  
  
  virtual void HandleMouseMove() {}
  virtual void HandleMouseDown() {}
  virtual void HandleMouseLeave() {}
  virtual void HandleMouseUp() {}
  virtual void HandlePointerActivate() {}
  virtual void HandleGestureBegin() {}
  virtual void HandleGestureEnd() {}

  
  virtual void RunFileChooser(
      RenderViewHost* render_view_host,
      const FileChooserParams& params) {}

  
  virtual void ToggleFullscreenMode(bool enter_fullscreen) {}
  virtual bool IsFullscreenForCurrentTab() const;

  
  virtual void UpdatePreferredSize(const gfx::Size& pref_size) {}

  
  virtual void ResizeDueToAutoResize(const gfx::Size& new_size) {}

  
  
  
  virtual void RequestToLockMouse(bool user_gesture,
                                  bool last_unlocked_by_target) {}

  
  virtual void LostMouseLock() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void CreateNewWindow(
      int render_process_id,
      int route_id,
      int main_frame_route_id,
      const ViewHostMsg_CreateWindow_Params& params,
      SessionStorageNamespace* session_storage_namespace) {}

  
  
  
  
  
  
  virtual void CreateNewWidget(int render_process_id,
                               int route_id,
                               blink::WebPopupType popup_type) {}

  
  virtual void CreateNewFullscreenWidget(int render_process_id, int route_id) {}

  
  
  
  
  
  virtual void ShowCreatedWindow(int route_id,
                                 WindowOpenDisposition disposition,
                                 const gfx::Rect& initial_pos,
                                 bool user_gesture) {}

  
  
  virtual void ShowCreatedWidget(int route_id,
                                 const gfx::Rect& initial_pos) {}

  
  virtual void ShowCreatedFullscreenWidget(int route_id) {}

  
  
  virtual void ShowContextMenu(const ContextMenuParams& params) {}

  
  
  
  virtual void RequestMediaAccessPermission(
      const MediaStreamRequest& request,
      const MediaResponseCallback& callback) {}

  
  
  virtual SessionStorageNamespace* GetSessionStorageNamespace(
      SiteInstance* instance);

  
  
  
  
  
  virtual FrameTree* GetFrameTree();

 protected:
  virtual ~RenderViewHostDelegate() {}
};

}  

#endif  
