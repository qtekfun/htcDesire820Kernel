// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_BROWSER_PLUGIN_BROWSER_PLUGIN_GUEST_H_
#define CONTENT_BROWSER_BROWSER_PLUGIN_BROWSER_PLUGIN_GUEST_H_

#include <map>
#include <queue>

#include "base/compiler_specific.h"
#include "base/id_map.h"
#include "base/memory/shared_memory.h"
#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "content/common/edit_command.h"
#include "content/port/common/input_event_ack_state.h"
#include "content/public/browser/browser_plugin_guest_delegate.h"
#include "content/public/browser/javascript_dialog_manager.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/common/browser_plugin_permission_type.h"
#include "third_party/WebKit/public/web/WebCompositionUnderline.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"
#include "third_party/WebKit/public/web/WebDragStatus.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"
#include "ui/base/ime/text_input_mode.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/gfx/rect.h"
#include "ui/surface/transport_dib.h"

struct BrowserPluginHostMsg_AutoSize_Params;
struct BrowserPluginHostMsg_Attach_Params;
struct BrowserPluginHostMsg_ResizeGuest_Params;
struct ViewHostMsg_CreateWindow_Params;
#if defined(OS_MACOSX)
struct ViewHostMsg_ShowPopup_Params;
#endif
struct ViewHostMsg_UpdateRect_Params;
class WebCursor;

namespace cc {
class CompositorFrameAck;
}

namespace blink {
class WebInputEvent;
}

namespace gfx {
class Range;
}

namespace content {

class BrowserPluginHostFactory;
class BrowserPluginEmbedder;
class BrowserPluginGuestManager;
class RenderProcessHost;
class RenderWidgetHostView;
class SiteInstance;
struct DropData;
struct MediaStreamRequest;

class CONTENT_EXPORT BrowserPluginGuest
    : public JavaScriptDialogManager,
      public WebContentsDelegate,
      public WebContentsObserver,
      public base::SupportsWeakPtr<BrowserPluginGuest> {
 public:
  typedef base::Callback<void(bool)> GeolocationCallback;
  virtual ~BrowserPluginGuest();

  
  
  
  
  
  
  
  static BrowserPluginGuest* Create(
      int instance_id,
      SiteInstance* guest_site_instance,
      WebContentsImpl* web_contents,
      scoped_ptr<base::DictionaryValue> extra_params);

  static BrowserPluginGuest* CreateWithOpener(
      int instance_id,
      bool has_render_view,
      WebContentsImpl* web_contents,
      BrowserPluginGuest* opener);

  
  
  void EmbedderDestroyed();

  
  void EmbedderVisibilityChanged(bool visible);

  
  
  void Destroy();

  
  
  int instance_id() const { return instance_id_; }

  bool OnMessageReceivedFromEmbedder(const IPC::Message& message);

  void Initialize(const BrowserPluginHostMsg_Attach_Params& params,
                  WebContentsImpl* embedder_web_contents);

  WebContentsImpl* embedder_web_contents() const {
    return embedder_web_contents_;
  }

  RenderWidgetHostView* GetEmbedderRenderWidgetHostView();

  bool focused() const { return focused_; }
  bool visible() const { return guest_visible_; }
  void clear_damage_buffer() { damage_buffer_.reset(); }
  bool is_in_destruction() { return is_in_destruction_; }

  BrowserPluginGuest* opener() const { return opener_.get(); }

  
  bool UnlockMouseIfNecessary(const NativeWebKeyboardEvent& event);

  void UpdateVisibility();

  void CopyFromCompositingSurface(
      gfx::Rect src_subrect,
      gfx::Size dst_size,
      const base::Callback<void(bool, const SkBitmap&)>& callback);

  
  virtual void DidCommitProvisionalLoadForFrame(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& url,
      PageTransition transition_type,
      RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidStopLoading(RenderViewHost* render_view_host) OVERRIDE;

  virtual void RenderViewReady() OVERRIDE;
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual bool AddMessageToConsole(WebContents* source,
                                   int32 level,
                                   const base::string16& message,
                                   int32 line_no,
                                   const base::string16& source_id) OVERRIDE;
  
  
  
  virtual void AddNewContents(WebContents* source,
                              WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;
  virtual void CanDownload(RenderViewHost* render_view_host,
                           int request_id,
                           const std::string& request_method,
                           const base::Callback<void(bool)>& callback) OVERRIDE;
  virtual void LoadProgressChanged(WebContents* source,
                                   double progress) OVERRIDE;
  virtual void CloseContents(WebContents* source) OVERRIDE;
  virtual JavaScriptDialogManager* GetJavaScriptDialogManager() OVERRIDE;
  virtual bool HandleContextMenu(const ContextMenuParams& params) OVERRIDE;
  virtual void HandleKeyboardEvent(
      WebContents* source,
      const NativeWebKeyboardEvent& event) OVERRIDE;
  virtual WebContents* OpenURLFromTab(WebContents* source,
                                      const OpenURLParams& params) OVERRIDE;
  virtual void WebContentsCreated(WebContents* source_contents,
                                  int64 source_frame_id,
                                  const base::string16& frame_name,
                                  const GURL& target_url,
                                  WebContents* new_contents) OVERRIDE;
  virtual void RendererUnresponsive(WebContents* source) OVERRIDE;
  virtual void RendererResponsive(WebContents* source) OVERRIDE;
  virtual void RunFileChooser(WebContents* web_contents,
                              const FileChooserParams& params) OVERRIDE;
  virtual bool ShouldFocusPageAfterCrash() OVERRIDE;
  virtual void RequestMediaAccessPermission(
      WebContents* web_contents,
      const MediaStreamRequest& request,
      const MediaResponseCallback& callback) OVERRIDE;

  
  virtual void RunJavaScriptDialog(
      WebContents* web_contents,
      const GURL& origin_url,
      const std::string& accept_lang,
      JavaScriptMessageType javascript_message_type,
      const base::string16& message_text,
      const base::string16& default_prompt_text,
      const DialogClosedCallback& callback,
      bool* did_suppress_message) OVERRIDE;
  virtual void RunBeforeUnloadDialog(
      WebContents* web_contents,
      const base::string16& message_text,
      bool is_reload,
      const DialogClosedCallback& callback) OVERRIDE;
  virtual bool HandleJavaScriptDialog(
      WebContents* web_contents,
      bool accept,
      const base::string16* prompt_override) OVERRIDE;
  virtual void CancelActiveAndPendingDialogs(
      WebContents* web_contents) OVERRIDE;
  virtual void WebContentsDestroyed(WebContents* web_contents) OVERRIDE;

  
  WebContentsImpl* GetWebContents();

  
  virtual void SetDamageBuffer(
      const BrowserPluginHostMsg_ResizeGuest_Params& params);

  gfx::Point GetScreenCoordinates(const gfx::Point& relative_position) const;

  
  
  
  
  virtual void SendMessageToEmbedder(IPC::Message* msg);

  
  bool attached() const { return embedder_web_contents_ != NULL; }

  
  
  
  
  
  
  void Attach(WebContentsImpl* embedder_web_contents,
              BrowserPluginHostMsg_Attach_Params params,
              const base::DictionaryValue& extra_params);

  
  void AskEmbedderForGeolocationPermission(int bridge_id,
                                           const GURL& requesting_frame,
                                           const GeolocationCallback& callback);
  
  void CancelGeolocationRequest(int bridge_id);

  
  
  static void AcknowledgeBufferPresent(int route_id,
                                       int gpu_host_id,
                                       const std::string& mailbox_name,
                                       uint32 sync_point);

  
  
  static bool ShouldForwardToBrowserPluginGuest(const IPC::Message& message);
  gfx::Rect ToGuestRect(const gfx::Rect& rect);

  void DragSourceEndedAt(int client_x, int client_y, int screen_x,
      int screen_y, blink::WebDragOperation operation);

  void DragSourceMovedTo(int client_x, int client_y,
                         int screen_x, int screen_y);

  
  void EndSystemDrag();

  
  void SetDelegate(BrowserPluginGuestDelegate* delegate);

  void RespondToPermissionRequest(int request_id,
                                  bool should_allow,
                                  const std::string& user_input);

  
  
  static void set_factory_for_testing(BrowserPluginHostFactory* factory) {
    BrowserPluginGuest::factory_ = factory;
  }

 private:
  class EmbedderWebContentsObserver;
  friend class TestBrowserPluginGuest;

  class DownloadRequest;
  class GeolocationRequest;
  class JavaScriptDialogRequest;
  
  class MediaRequest;
  class NewWindowRequest;
  class PermissionRequest;
  class PointerLockRequest;

  
  
  
  
  struct NewWindowInfo {
    bool changed;
    GURL url;
    std::string name;
    NewWindowInfo(const GURL& url, const std::string& name) :
        changed(false),
        url(url),
        name(name) {}
  };

  
  
  BrowserPluginGuest(int instance_id,
                     bool has_render_view,
                     WebContentsImpl* web_contents,
                     BrowserPluginGuest* opener);

  
  
  void DestroyUnattachedWindows();

  void LoadURLWithParams(const GURL& url,
                         const Referrer& referrer,
                         PageTransition transition_type,
                         WebContents* web_contents);

  
  
  
  int RemoveBridgeID(int bridge_id);

  
  int RequestPermission(
      BrowserPluginPermissionType permission_type,
      scoped_refptr<BrowserPluginGuest::PermissionRequest> request,
      const base::DictionaryValue& request_info);

  
  
  BrowserPluginGuest* CreateNewGuestWindow(const OpenURLParams& params);

  base::SharedMemory* damage_buffer() const { return damage_buffer_.get(); }
  const gfx::Size& damage_view_size() const { return damage_view_size_; }
  float damage_buffer_scale_factor() const {
    return damage_buffer_scale_factor_;
  }
  
  base::SharedMemory* GetDamageBufferFromEmbedder(
      const BrowserPluginHostMsg_ResizeGuest_Params& params);

  bool InAutoSizeBounds(const gfx::Size& size) const;

  void RequestNewWindowPermission(WindowOpenDisposition disposition,
                                  const gfx::Rect& initial_bounds,
                                  bool user_gesture,
                                  WebContentsImpl* new_contents);

  

  void OnCompositorFrameACK(int instance_id,
                            int route_id,
                            uint32 output_surface_id,
                            int renderer_host_id,
                            const cc::CompositorFrameAck& ack);
  void OnCopyFromCompositingSurfaceAck(int instance_id,
                                       int request_id,
                                       const SkBitmap& bitmap);
  
  
  
  
  
  void OnDragStatusUpdate(int instance_id,
                          blink::WebDragStatus drag_status,
                          const DropData& drop_data,
                          blink::WebDragOperationsMask drag_mask,
                          const gfx::Point& location);
  
  void OnExecuteEditCommand(int instance_id,
                            const std::string& command);

  
  void OnReclaimCompositorResources(int instance_id,
                                    int route_id,
                                    uint32 output_surface_id,
                                    int renderer_host_id,
                                    const cc::CompositorFrameAck& ack);

  
  virtual void OnHandleInputEvent(int instance_id,
                                  const gfx::Rect& guest_window_rect,
                                  const blink::WebInputEvent* event);
  void OnLockMouse(bool user_gesture,
                   bool last_unlocked_by_target,
                   bool privileged);
  void OnLockMouseAck(int instance_id, bool succeeded);
  void OnNavigateGuest(int instance_id, const std::string& src);
  void OnPluginDestroyed(int instance_id);
  
  
  void OnResizeGuest(int instance_id,
                     const BrowserPluginHostMsg_ResizeGuest_Params& params);
  
  virtual void OnSetFocus(int instance_id, bool focused);
  
  
  void OnSetName(int instance_id, const std::string& name);
  
  void OnSetSize(
      int instance_id,
      const BrowserPluginHostMsg_AutoSize_Params& auto_size_params,
      const BrowserPluginHostMsg_ResizeGuest_Params& resize_guest_params);
  void OnSetEditCommandsForNextKeyEvent(
      int instance_id,
      const std::vector<EditCommand>& edit_commands);
  void OnSetContentsOpaque(int instance_id, bool opaque);
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void OnSetVisibility(int instance_id, bool visible);
  
  void OnSwapBuffersACK(int instance_id,
                        int route_id,
                        int gpu_host_id,
                        const std::string& mailbox_name,
                        uint32 sync_point);
  void OnUnlockMouse();
  void OnUnlockMouseAck(int instance_id);
  void OnUpdateGeometry(int instance_id, const gfx::Rect& view_rect);
  void OnUpdateRectACK(
      int instance_id,
      bool needs_ack,
      const BrowserPluginHostMsg_AutoSize_Params& auto_size_params,
      const BrowserPluginHostMsg_ResizeGuest_Params& resize_guest_params);

  void OnTextInputTypeChanged(ui::TextInputType type,
                              ui::TextInputMode input_mode,
                              bool can_compose_inline);
  void OnImeSetComposition(
      int instance_id,
      const std::string& text,
      const std::vector<blink::WebCompositionUnderline>& underlines,
      int selection_start,
      int selection_end);
  void OnImeConfirmComposition(
      int instance_id,
      const std::string& text,
      bool keep_selection);
  void OnExtendSelectionAndDelete(int instance_id, int before, int after);
  
  virtual void OnImeCancelComposition();
#if defined(OS_MACOSX) || defined(OS_WIN) || defined(USE_AURA)
  void OnImeCompositionRangeChanged(
      const gfx::Range& range,
      const std::vector<gfx::Rect>& character_bounds);
#endif

  

  void OnDragStopped();
  void OnHandleInputEventAck(
      blink::WebInputEvent::Type event_type,
      InputEventAckState ack_result);
  void OnHasTouchEventHandlers(bool accept);
  void OnSetCursor(const WebCursor& cursor);
  
  
#if defined(OS_MACOSX)
  void OnShowPopup(const ViewHostMsg_ShowPopup_Params& params);
#endif
  void OnShowWidget(int route_id, const gfx::Rect& initial_pos);
  
  virtual void OnTakeFocus(bool reverse);
  void OnUpdateFrameName(int frame_id,
                         bool is_top_level,
                         const std::string& name);
  void OnUpdateRect(const ViewHostMsg_UpdateRect_Params& params);

  
  
  void DidRetrieveDownloadURLFromRequestId(
      const std::string& request_method,
      const base::Callback<void(bool)>& callback,
      const std::string& url);

  
  void SetGeolocationPermission(
      GeolocationCallback callback, int bridge_id, bool allowed);

  
  void SendQueuedMessages();

  
  
  base::WeakPtrFactory<BrowserPluginGuest> weak_ptr_factory_;

  
  static BrowserPluginHostFactory* factory_;

  scoped_ptr<EmbedderWebContentsObserver> embedder_web_contents_observer_;
  WebContentsImpl* embedder_web_contents_;

  std::map<int, int> bridge_id_to_request_id_map_;

  
  
  int instance_id_;
  scoped_ptr<base::SharedMemory> damage_buffer_;
  
  uint32 damage_buffer_sequence_id_;
  size_t damage_buffer_size_;
  gfx::Size damage_view_size_;
  float damage_buffer_scale_factor_;
  float guest_device_scale_factor_;
  gfx::Rect guest_window_rect_;
  gfx::Rect guest_screen_rect_;
  base::TimeDelta guest_hang_timeout_;
  bool focused_;
  bool mouse_locked_;
  bool pending_lock_request_;
  bool guest_visible_;
  bool guest_opaque_;
  bool embedder_visible_;
  std::string name_;
  bool auto_size_enabled_;
  gfx::Size max_auto_size_;
  gfx::Size min_auto_size_;

  
  
  int copy_request_id_;
  typedef base::Callback<void(bool, const SkBitmap&)> CopyRequestCallback;
  typedef std::map<int, const CopyRequestCallback> CopyRequestMap;
  CopyRequestMap copy_request_callbacks_;

  typedef std::map<BrowserPluginGuest*, NewWindowInfo> PendingWindowMap;
  PendingWindowMap pending_new_windows_;
  base::WeakPtr<BrowserPluginGuest> opener_;
  
  
  int next_permission_request_id_;

  
  typedef std::map<int, scoped_refptr<PermissionRequest> > RequestMap;
  RequestMap permission_request_map_;

  
  
  
  bool has_render_view_;

  
  gfx::Size last_seen_view_size_;
  
  bool last_seen_auto_size_enabled_;

  bool is_in_destruction_;

  
  
  std::queue<IPC::Message*> pending_messages_;

  scoped_ptr<BrowserPluginGuestDelegate> delegate_;

  
  
  scoped_ptr<base::DictionaryValue> extra_attach_params_;

  DISALLOW_COPY_AND_ASSIGN(BrowserPluginGuest);
};

}  

#endif  
