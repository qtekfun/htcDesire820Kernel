// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_VIEW_HOST_IMPL_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_VIEW_HOST_IMPL_H_

#include <map>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/kill.h"
#include "content/browser/frame_host/render_frame_host_impl.h"
#include "content/browser/renderer_host/render_widget_host_impl.h"
#include "content/browser/site_instance_impl.h"
#include "content/common/accessibility_node_data.h"
#include "content/common/drag_event_source_info.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/common/javascript_message_type.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/common/window_container_type.h"
#include "net/base/load_states.h"
#include "third_party/WebKit/public/web/WebAXEnums.h"
#include "third_party/WebKit/public/web/WebConsoleMessage.h"
#include "third_party/WebKit/public/web/WebPopupType.h"
#include "third_party/WebKit/public/web/WebTextDirection.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/window_open_disposition.h"

class SkBitmap;
class ViewMsg_Navigate;
struct AccessibilityHostMsg_EventParams;
struct MediaPlayerAction;
struct ViewHostMsg_CreateWindow_Params;
struct ViewHostMsg_DidFailProvisionalLoadWithError_Params;
struct ViewHostMsg_OpenURL_Params;
struct ViewHostMsg_SelectionBounds_Params;
struct ViewHostMsg_ShowPopup_Params;
struct ViewMsg_Navigate_Params;
struct ViewMsg_PostMessage_Params;
struct ViewMsg_StopFinding_Params;

namespace base {
class ListValue;
}

namespace gfx {
class Range;
}

namespace ui {
struct SelectedFileInfo;
}

namespace content {

class BrowserMediaPlayerManager;
class ChildProcessSecurityPolicyImpl;
class PageState;
class RenderFrameHostDelegate;
class RenderFrameHostImpl;
class RenderWidgetHostDelegate;
class SessionStorageNamespace;
class SessionStorageNamespaceImpl;
class TestRenderViewHost;
struct ContextMenuParams;
struct FileChooserParams;
struct Referrer;
struct ShowDesktopNotificationHostMsgParams;

#if defined(COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable: 4250)
#endif

class CONTENT_EXPORT RenderViewHostImpl
    : public RenderViewHost,
      public RenderWidgetHostImpl {
 public:
  
  static RenderViewHostImpl* FromID(int render_process_id, int render_view_id);

  
  
  
  
  
  
  
  
  
  
  
  RenderViewHostImpl(
      SiteInstance* instance,
      RenderViewHostDelegate* delegate,
      RenderFrameHostDelegate* frame_delegate,
      RenderWidgetHostDelegate* widget_delegate,
      int routing_id,
      int main_frame_routing_id,
      bool swapped_out,
      bool hidden);
  virtual ~RenderViewHostImpl();

  
  virtual void AllowBindings(int binding_flags) OVERRIDE;
  virtual void ClearFocusedNode() OVERRIDE;
  virtual void ClosePage() OVERRIDE;
  virtual void CopyImageAt(int x, int y) OVERRIDE;
  virtual void DesktopNotificationPermissionRequestDone(
      int callback_context) OVERRIDE;
  virtual void DesktopNotificationPostDisplay(int callback_context) OVERRIDE;
  virtual void DesktopNotificationPostError(
      int notification_id,
      const base::string16& message) OVERRIDE;
  virtual void DesktopNotificationPostClose(int notification_id,
                                            bool by_user) OVERRIDE;
  virtual void DesktopNotificationPostClick(int notification_id) OVERRIDE;
  virtual void DirectoryEnumerationFinished(
      int request_id,
      const std::vector<base::FilePath>& files) OVERRIDE;
  virtual void DisableScrollbarsForThreshold(const gfx::Size& size) OVERRIDE;
  virtual void DragSourceEndedAt(
      int client_x, int client_y, int screen_x, int screen_y,
      blink::WebDragOperation operation) OVERRIDE;
  virtual void DragSourceMovedTo(
      int client_x, int client_y, int screen_x, int screen_y) OVERRIDE;
  virtual void DragSourceSystemDragEnded() OVERRIDE;
  virtual void DragTargetDragEnter(
      const DropData& drop_data,
      const gfx::Point& client_pt,
      const gfx::Point& screen_pt,
      blink::WebDragOperationsMask operations_allowed,
      int key_modifiers) OVERRIDE;
  virtual void DragTargetDragOver(
      const gfx::Point& client_pt,
      const gfx::Point& screen_pt,
      blink::WebDragOperationsMask operations_allowed,
      int key_modifiers) OVERRIDE;
  virtual void DragTargetDragLeave() OVERRIDE;
  virtual void DragTargetDrop(const gfx::Point& client_pt,
                              const gfx::Point& screen_pt,
                              int key_modifiers) OVERRIDE;
  virtual void EnableAutoResize(const gfx::Size& min_size,
                                const gfx::Size& max_size) OVERRIDE;
  virtual void DisableAutoResize(const gfx::Size& new_size) OVERRIDE;
  virtual void EnablePreferredSizeMode() OVERRIDE;
  virtual void ExecuteCustomContextMenuCommand(
      int action, const CustomContextMenuContext& context) OVERRIDE;
  virtual void ExecuteMediaPlayerActionAtLocation(
      const gfx::Point& location,
      const blink::WebMediaPlayerAction& action) OVERRIDE;
  virtual void ExecuteJavascriptInWebFrame(
      const base::string16& frame_xpath,
      const base::string16& jscript) OVERRIDE;
  virtual void ExecuteJavascriptInWebFrameCallbackResult(
      const base::string16& frame_xpath,
      const base::string16& jscript,
      const JavascriptResultCallback& callback) OVERRIDE;
  virtual void ExecutePluginActionAtLocation(
      const gfx::Point& location,
      const blink::WebPluginAction& action) OVERRIDE;
  virtual void ExitFullscreen() OVERRIDE;
  virtual void Find(int request_id, const base::string16& search_text,
                    const blink::WebFindOptions& options) OVERRIDE;
  virtual void StopFinding(StopFindAction action) OVERRIDE;
  virtual void FirePageBeforeUnload(bool for_cross_site_transition) OVERRIDE;
  virtual void FilesSelectedInChooser(
      const std::vector<ui::SelectedFileInfo>& files,
      FileChooserParams::Mode permissions) OVERRIDE;
  virtual RenderViewHostDelegate* GetDelegate() const OVERRIDE;
  virtual int GetEnabledBindings() const OVERRIDE;
  virtual SiteInstance* GetSiteInstance() const OVERRIDE;
  virtual void InsertCSS(const base::string16& frame_xpath,
                         const std::string& css) OVERRIDE;
  virtual bool IsRenderViewLive() const OVERRIDE;
  virtual bool IsSubframe() const OVERRIDE;
  virtual void NotifyContextMenuClosed(
      const CustomContextMenuContext& context) OVERRIDE;
  virtual void NotifyMoveOrResizeStarted() OVERRIDE;
  virtual void ReloadFrame() OVERRIDE;
  virtual void SetAltErrorPageURL(const GURL& url) OVERRIDE;
  virtual void SetWebUIProperty(const std::string& name,
                                const std::string& value) OVERRIDE;
  virtual void Zoom(PageZoom zoom) OVERRIDE;
  virtual void SyncRendererPrefs() OVERRIDE;
  virtual void ToggleSpeechInput() OVERRIDE;
  virtual WebPreferences GetWebkitPreferences() OVERRIDE;
  virtual void UpdateWebkitPreferences(
      const WebPreferences& prefs) OVERRIDE;
  virtual void NotifyTimezoneChange() OVERRIDE;
  virtual void GetAudioOutputControllers(
      const GetAudioOutputControllersCallback& callback) const OVERRIDE;

#if defined(OS_ANDROID)
  virtual void ActivateNearestFindResult(int request_id,
                                         float x,
                                         float y) OVERRIDE;
  virtual void RequestFindMatchRects(int current_version) OVERRIDE;
  virtual void DisableFullscreenEncryptedMediaPlayback() OVERRIDE;
#endif

  void set_delegate(RenderViewHostDelegate* d) {
    CHECK(d);  
    delegate_ = d;
  }

  
  
  
  
  
  
  virtual bool CreateRenderView(const base::string16& frame_name,
                                int opener_route_id,
                                int32 max_page_id);

  base::TerminationStatus render_view_termination_status() const {
    return render_view_termination_status_;
  }

  
  WebPreferences GetWebkitPrefs(const GURL& url);

  
  
  
  
  
  
  
  void Navigate(const ViewMsg_Navigate_Params& message);

  
  void NavigateToURL(const GURL& url);

  
  
  
  bool are_navigations_suspended() const { return navigations_suspended_; }

  
  
  
  
  
  
  
  
  
  
  
  void SetNavigationsSuspended(bool suspend,
                               const base::TimeTicks& proceed_time);

  
  
  
  void CancelSuspendedNavigations();

  
  
  
  bool has_accessed_initial_document() {
    return has_accessed_initial_document_;
  }

  
  
  bool is_swapped_out() const { return is_swapped_out_; }

  
  
  
  
  void OnCrossSiteResponse(
      const GlobalRequestID& global_request_id,
      bool is_transfer,
      const std::vector<GURL>& transfer_url_chain,
      const Referrer& referrer,
      PageTransition page_transition,
      int64 frame_id,
      bool should_replace_current_entry);

  
  
  
  
  void SuppressDialogsUntilSwapOut();

  
  
  
  
  
  
  void SwapOut();

  
  
  void OnSwappedOut(bool timed_out);

  
  
  
  
  void WasSwappedOut();

  
  
  
  void ClosePageIgnoringUnloadEvents();

  
  
  
  bool HasPendingCrossSiteRequest();

  
  
  
  
  void SetHasPendingCrossSiteRequest(bool has_pending_request);

  
  
  void JavaScriptDialogClosed(IPC::Message* reply_msg,
                              bool success,
                              const base::string16& user_input);

  
  void SetInitialFocus(bool reverse);

  
  
  
  
  
  
  
  void GetSerializedHtmlDataForCurrentPageWithLocalLinks(
      const std::vector<GURL>& links,
      const std::vector<base::FilePath>& local_paths,
      const base::FilePath& local_directory_name);

  
  void LoadStateChanged(const GURL& url,
                        const net::LoadStateWithParam& load_state,
                        uint64 upload_position,
                        uint64 upload_size);

  bool SuddenTerminationAllowed() const;
  void set_sudden_termination_allowed(bool enabled) {
    sudden_termination_allowed_ = enabled;
  }

  
  virtual void Init() OVERRIDE;
  virtual void Shutdown() OVERRIDE;
  virtual bool IsRenderView() const OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void GotFocus() OVERRIDE;
  virtual void LostCapture() OVERRIDE;
  virtual void LostMouseLock() OVERRIDE;
  virtual void ForwardMouseEvent(
      const blink::WebMouseEvent& mouse_event) OVERRIDE;
  virtual void OnPointerEventActivate() OVERRIDE;
  virtual void ForwardKeyboardEvent(
      const NativeWebKeyboardEvent& key_event) OVERRIDE;
  virtual gfx::Rect GetRootWindowResizerRect() const OVERRIDE;

  
  void CreateNewWindow(
      int route_id,
      int main_frame_route_id,
      const ViewHostMsg_CreateWindow_Params& params,
      SessionStorageNamespace* session_storage_namespace);

  
  
  void CreateNewWidget(int route_id, blink::WebPopupType popup_type);

  
  void CreateNewFullscreenWidget(int route_id);

#if defined(OS_MACOSX)
  
  void DidSelectPopupMenuItem(int selected_index);
  void DidCancelPopupMenu();
#endif

#if defined(OS_ANDROID)
  BrowserMediaPlayerManager* media_player_manager() {
    return media_player_manager_.get();
  }

  void DidSelectPopupMenuItems(const std::vector<int>& selected_indices);
  void DidCancelPopupMenu();
#endif

  
  void SendOrientationChangeEvent(int orientation);

  
  
  void set_is_subframe(bool is_subframe) {
    is_subframe_ = is_subframe;
  }

  
  
  int64 main_frame_id() const {
    return main_frame_id_;
  }

  
  void DisownOpener();

  
  
  
  
  void SetAccessibilityCallbackForTesting(
      const base::Callback<void(blink::WebAXEvent)>& callback);

  
  
  
  
  const AccessibilityNodeDataTreeNode& accessibility_tree_for_testing() {
    return accessibility_tree_;
  }

  
  void SetAccessibilityLayoutCompleteCallbackForTesting(
      const base::Closure& callback);
  void SetAccessibilityLoadCompleteCallbackForTesting(
      const base::Closure& callback);
  void SetAccessibilityOtherCallbackForTesting(
      const base::Closure& callback);

  bool is_waiting_for_beforeunload_ack() {
    return is_waiting_for_beforeunload_ack_;
  }

  bool is_waiting_for_unload_ack() {
    return is_waiting_for_unload_ack_;
  }

  
  
  
  bool CanCommitURL(const GURL& url);

  
  
  
  static void FilterURL(ChildProcessSecurityPolicyImpl* policy,
                        const RenderProcessHost* process,
                        bool empty_allowed,
                        GURL* url);

  
  
  
  
  
  void AttachToFrameTree();

  
  
  
  
  
  void OnDidStartProvisionalLoadForFrame(int64 frame_id,
                                         int64 parent_frame_id,
                                         bool main_frame,
                                         const GURL& url);

  
  
  
  

 protected:
  
  virtual void OnUserGesture() OVERRIDE;
  virtual void NotifyRendererUnresponsive() OVERRIDE;
  virtual void NotifyRendererResponsive() OVERRIDE;
  virtual void OnRenderAutoResized(const gfx::Size& size) OVERRIDE;
  virtual void RequestToLockMouse(bool user_gesture,
                                  bool last_unlocked_by_target) OVERRIDE;
  virtual bool IsFullscreen() const OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

  
  void OnShowView(int route_id,
                  WindowOpenDisposition disposition,
                  const gfx::Rect& initial_pos,
                  bool user_gesture);
  void OnShowWidget(int route_id, const gfx::Rect& initial_pos);
  void OnShowFullscreenWidget(int route_id);
  void OnRunModal(int opener_id, IPC::Message* reply_msg);
  void OnRenderViewReady();
  void OnRenderProcessGone(int status, int error_code);
  void OnDidRedirectProvisionalLoad(int32 page_id,
                                    const GURL& source_url,
                                    const GURL& target_url);
  void OnDidFailProvisionalLoadWithError(
      const ViewHostMsg_DidFailProvisionalLoadWithError_Params& params);
  void OnNavigate(const IPC::Message& msg);
  void OnUpdateState(int32 page_id, const PageState& state);
  void OnUpdateTitle(int32 page_id,
                     const base::string16& title,
                     blink::WebTextDirection title_direction);
  void OnUpdateEncoding(const std::string& encoding);
  void OnUpdateTargetURL(int32 page_id, const GURL& url);
  void OnClose();
  void OnRequestMove(const gfx::Rect& pos);
  void OnDidStartLoading();
  void OnDidStopLoading();
  void OnDidChangeLoadProgress(double load_progress);
  void OnDidDisownOpener();
  void OnDocumentAvailableInMainFrame();
  void OnDocumentOnLoadCompletedInMainFrame(int32 page_id);
  void OnContextMenu(const ContextMenuParams& params);
  void OnToggleFullscreen(bool enter_fullscreen);
  void OnOpenURL(const ViewHostMsg_OpenURL_Params& params);
  void OnDidContentsPreferredSizeChange(const gfx::Size& new_size);
  void OnDidChangeScrollOffset();
  void OnDidChangeScrollbarsForMainFrame(bool has_horizontal_scrollbar,
                                         bool has_vertical_scrollbar);
  void OnDidChangeScrollOffsetPinningForMainFrame(bool is_pinned_to_left,
                                                  bool is_pinned_to_right);
  void OnDidChangeNumWheelEvents(int count);
  void OnSelectionChanged(const base::string16& text,
                          size_t offset,
                          const gfx::Range& range);
  void OnSelectionBoundsChanged(
      const ViewHostMsg_SelectionBounds_Params& params);
  void OnPasteFromSelectionClipboard();
  void OnRouteCloseEvent();
  void OnRouteMessageEvent(const ViewMsg_PostMessage_Params& params);
  void OnRunJavaScriptMessage(const base::string16& message,
                              const base::string16& default_prompt,
                              const GURL& frame_url,
                              JavaScriptMessageType type,
                              IPC::Message* reply_msg);
  void OnRunBeforeUnloadConfirm(const GURL& frame_url,
                                const base::string16& message,
                                bool is_reload,
                                IPC::Message* reply_msg);
  void OnStartDragging(const DropData& drop_data,
                       blink::WebDragOperationsMask operations_allowed,
                       const SkBitmap& bitmap,
                       const gfx::Vector2d& bitmap_offset_in_dip,
                       const DragEventSourceInfo& event_info);
  void OnUpdateDragCursor(blink::WebDragOperation drag_operation);
  void OnTargetDropACK();
  void OnTakeFocus(bool reverse);
  void OnFocusedNodeChanged(bool is_editable_node);
  void OnAddMessageToConsole(int32 level,
                             const base::string16& message,
                             int32 line_no,
                             const base::string16& source_id);
  void OnUpdateInspectorSetting(const std::string& key,
                                const std::string& value);
  void OnShouldCloseACK(
      bool proceed,
      const base::TimeTicks& renderer_before_unload_start_time,
      const base::TimeTicks& renderer_before_unload_end_time);
  void OnClosePageACK();
  void OnSwapOutACK();
  void OnAccessibilityEvents(
      const std::vector<AccessibilityHostMsg_EventParams>& params);
  void OnScriptEvalResponse(int id, const base::ListValue& result);
  void OnDidZoomURL(double zoom_level, bool remember, const GURL& url);
  void OnRequestDesktopNotificationPermission(const GURL& origin,
                                              int callback_id);
  void OnShowDesktopNotification(
      const ShowDesktopNotificationHostMsgParams& params);
  void OnCancelDesktopNotification(int notification_id);
  void OnRunFileChooser(const FileChooserParams& params);
  void OnDidAccessInitialDocument();
  void OnDomOperationResponse(const std::string& json_string,
                              int automation_id);
  void OnFocusedNodeTouched(bool editable);

#if defined(OS_MACOSX) || defined(OS_ANDROID)
  void OnShowPopup(const ViewHostMsg_ShowPopup_Params& params);
#endif

  
  
  RenderFrameHostImpl* main_render_frame_host() const {
    return main_render_frame_host_.get();
  }

 private:
  friend class TestRenderViewHost;
  FRIEND_TEST_ALL_PREFIXES(RenderViewHostTest, BasicRenderFrameHost);
  FRIEND_TEST_ALL_PREFIXES(RenderViewHostTest, RoutingIdSane);

  
  
  void SetSwappedOut(bool is_swapped_out);

  bool CanAccessFilesOfPageState(const PageState& state) const;

  
  
  
  
  
  
  
  
  scoped_ptr<RenderFrameHostImpl> main_render_frame_host_;

  
  RenderViewHostDelegate* delegate_;

  
  
  
  scoped_refptr<SiteInstanceImpl> instance_;

  
  
  bool waiting_for_drag_context_response_;

  
  
  int enabled_bindings_;

  
  
  
  
  bool navigations_suspended_;

  
  
  
  
  scoped_ptr<ViewMsg_Navigate_Params> suspended_nav_params_;

  
  
  
  
  bool has_accessed_initial_document_;

  
  
  bool is_swapped_out_;

  
  
  bool is_subframe_;

  
  
  
  int64 main_frame_id_;

  
  
  IPC::Message* run_modal_reply_msg_;
  
  int run_modal_opener_id_;

  
  
  
  
  
  bool is_waiting_for_beforeunload_ack_;

  
  
  bool is_waiting_for_unload_ack_;

  
  bool has_timed_out_on_unload_;

  
  
  
  
  bool unload_ack_is_for_cross_site_transition_;

  bool are_javascript_messages_suppressed_;

  
  
  
  std::map<int, JavascriptResultCallback> javascript_callbacks_;

  
  base::Callback<void(blink::WebAXEvent)> accessibility_testing_callback_;

  
  AccessibilityNodeDataTreeNode accessibility_tree_;

  
  bool sudden_termination_allowed_;

  
  base::TerminationStatus render_view_termination_status_;

  
  base::TimeTicks send_should_close_start_time_;

  
  bool virtual_keyboard_requested_;

#if defined(OS_ANDROID)
  
  scoped_ptr<BrowserMediaPlayerManager> media_player_manager_;
#endif

  DISALLOW_COPY_AND_ASSIGN(RenderViewHostImpl);
};

#if defined(COMPILER_MSVC)
#pragma warning(pop)
#endif

}  

#endif  
