// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_IMPL_H_
#define CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_IMPL_H_

#include <map>
#include <set>
#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/process/process.h"
#include "base/values.h"
#include "content/browser/frame_host/frame_tree.h"
#include "content/browser/frame_host/navigation_controller_delegate.h"
#include "content/browser/frame_host/navigation_controller_impl.h"
#include "content/browser/frame_host/navigator_delegate.h"
#include "content/browser/frame_host/render_frame_host_delegate.h"
#include "content/browser/frame_host/render_frame_host_manager.h"
#include "content/browser/renderer_host/render_view_host_delegate.h"
#include "content/browser/renderer_host/render_widget_host_delegate.h"
#include "content/common/content_export.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/common/renderer_preferences.h"
#include "content/public/common/three_d_api_types.h"
#include "net/base/load_states.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/size.h"
#include "webkit/common/resource_type.h"

struct BrowserPluginHostMsg_ResizeGuest_Params;
struct ViewHostMsg_DateTimeDialogValue_Params;
struct ViewMsg_PostMessage_Params;

namespace content {
class BrowserPluginEmbedder;
class BrowserPluginGuest;
class BrowserPluginGuestManager;
class ColorChooser;
class DateTimeChooserAndroid;
class DownloadItem;
class InterstitialPageImpl;
class JavaBridgeDispatcherHostManager;
class JavaScriptDialogManager;
class PowerSaveBlocker;
class RenderViewHost;
class RenderViewHostDelegateView;
class RenderViewHostImpl;
class RenderWidgetHostImpl;
class RenderWidgetHostViewPort;
class SavePackage;
class SessionStorageNamespaceImpl;
class SiteInstance;
class TestWebContents;
class WebContentsDelegate;
class WebContentsImpl;
class WebContentsObserver;
class WebContentsViewPort;
class WebContentsViewDelegate;
struct ColorSuggestion;
struct FaviconURL;
struct LoadNotificationDetails;

WebContentsViewPort* CreateWebContentsView(
    WebContentsImpl* web_contents,
    WebContentsViewDelegate* delegate,
    RenderViewHostDelegateView** render_view_host_delegate_view);

class CONTENT_EXPORT WebContentsImpl
    : public NON_EXPORTED_BASE(WebContents),
      public NON_EXPORTED_BASE(RenderFrameHostDelegate),
      public RenderViewHostDelegate,
      public RenderWidgetHostDelegate,
      public RenderFrameHostManager::Delegate,
      public NotificationObserver,
      public NON_EXPORTED_BASE(NavigationControllerDelegate),
      public NON_EXPORTED_BASE(NavigatorDelegate) {
 public:
  virtual ~WebContentsImpl();

  static WebContentsImpl* CreateWithOpener(
      const WebContents::CreateParams& params,
      WebContentsImpl* opener);

  
  
  WebContentsImpl* opener() const { return opener_; }

  
  static BrowserPluginGuest* CreateGuest(
      BrowserContext* browser_context,
      content::SiteInstance* site_instance,
      int guest_instance_id,
      scoped_ptr<base::DictionaryValue> extra_params);

  
  
  
  
  int CreateSwappedOutRenderView(SiteInstance* instance);

  
  
  virtual void Init(const WebContents::CreateParams& params);

  
  SavePackage* save_package() const { return save_package_.get(); }

#if defined(OS_ANDROID)
  JavaBridgeDispatcherHostManager* java_bridge_dispatcher_host_manager() const {
    return java_bridge_dispatcher_host_manager_.get();
  }

  
  
  
  
  
  bool CreateRenderViewForInitialEmptyDocument();
#endif

  
  RenderFrameHostManager* GetRenderManagerForTesting();

  
  
  BrowserPluginGuest* GetBrowserPluginGuest() const;

  
  
  
  void SetBrowserPluginGuest(BrowserPluginGuest* guest);

  
  
  BrowserPluginEmbedder* GetBrowserPluginEmbedder() const;

  
  
  BrowserPluginGuestManager* GetBrowserPluginGuestManager() const;

  
  
  int GetFullscreenWidgetRoutingID() const;

  
  void DidChangeVisibleSSLState();

  
  
  void DragSourceEndedAt(int client_x, int client_y, int screen_x,
      int screen_y, blink::WebDragOperation operation);

  
  
  void DragSourceMovedTo(int client_x, int client_y,
                         int screen_x, int screen_y);

  
  virtual WebContentsDelegate* GetDelegate() OVERRIDE;
  virtual void SetDelegate(WebContentsDelegate* delegate) OVERRIDE;
  virtual NavigationControllerImpl& GetController() OVERRIDE;
  virtual const NavigationControllerImpl& GetController() const OVERRIDE;
  virtual BrowserContext* GetBrowserContext() const OVERRIDE;
  virtual RenderProcessHost* GetRenderProcessHost() const OVERRIDE;
  virtual RenderFrameHost* GetMainFrame() OVERRIDE;
  virtual RenderViewHost* GetRenderViewHost() const OVERRIDE;
  virtual void GetRenderViewHostAtPosition(
      int x,
      int y,
      const GetRenderViewHostCallback& callback) OVERRIDE;
  virtual WebContents* GetEmbedderWebContents() const OVERRIDE;
  virtual int GetEmbeddedInstanceID() const OVERRIDE;
  virtual int GetRoutingID() const OVERRIDE;
  virtual RenderWidgetHostView* GetRenderWidgetHostView() const OVERRIDE;
  virtual RenderWidgetHostView* GetFullscreenRenderWidgetHostView() const
      OVERRIDE;
  virtual WebContentsView* GetView() const OVERRIDE;
  virtual WebUI* CreateWebUI(const GURL& url) OVERRIDE;
  virtual WebUI* GetWebUI() const OVERRIDE;
  virtual WebUI* GetCommittedWebUI() const OVERRIDE;
  virtual void SetUserAgentOverride(const std::string& override) OVERRIDE;
  virtual const std::string& GetUserAgentOverride() const OVERRIDE;
#if defined(OS_WIN) && defined(USE_AURA)
  virtual void SetParentNativeViewAccessible(
      gfx::NativeViewAccessible accessible_parent) OVERRIDE;
#endif
  virtual const base::string16& GetTitle() const OVERRIDE;
  virtual int32 GetMaxPageID() OVERRIDE;
  virtual int32 GetMaxPageIDForSiteInstance(
      SiteInstance* site_instance) OVERRIDE;
  virtual SiteInstance* GetSiteInstance() const OVERRIDE;
  virtual SiteInstance* GetPendingSiteInstance() const OVERRIDE;
  virtual bool IsLoading() const OVERRIDE;
  virtual bool IsWaitingForResponse() const OVERRIDE;
  virtual const net::LoadStateWithParam& GetLoadState() const OVERRIDE;
  virtual const base::string16& GetLoadStateHost() const OVERRIDE;
  virtual uint64 GetUploadSize() const OVERRIDE;
  virtual uint64 GetUploadPosition() const OVERRIDE;
  virtual std::set<GURL> GetSitesInTab() const OVERRIDE;
  virtual const std::string& GetEncoding() const OVERRIDE;
  virtual bool DisplayedInsecureContent() const OVERRIDE;
  virtual void IncrementCapturerCount() OVERRIDE;
  virtual void DecrementCapturerCount() OVERRIDE;
  virtual int GetCapturerCount() const OVERRIDE;
  virtual bool IsCrashed() const OVERRIDE;
  virtual void SetIsCrashed(base::TerminationStatus status,
                            int error_code) OVERRIDE;
  virtual base::TerminationStatus GetCrashedStatus() const OVERRIDE;
  virtual bool IsBeingDestroyed() const OVERRIDE;
  virtual void NotifyNavigationStateChanged(unsigned changed_flags) OVERRIDE;
  virtual base::TimeTicks GetLastSelectedTime() const OVERRIDE;
  virtual void WasShown() OVERRIDE;
  virtual void WasHidden() OVERRIDE;
  virtual bool NeedToFireBeforeUnload() OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual WebContents* Clone() OVERRIDE;
  virtual void FocusThroughTabTraversal(bool reverse) OVERRIDE;
  virtual bool ShowingInterstitialPage() const OVERRIDE;
  virtual InterstitialPage* GetInterstitialPage() const OVERRIDE;
  virtual bool IsSavable() OVERRIDE;
  virtual void OnSavePage() OVERRIDE;
  virtual bool SavePage(const base::FilePath& main_file,
                        const base::FilePath& dir_path,
                        SavePageType save_type) OVERRIDE;
  virtual void SaveFrame(const GURL& url,
                         const Referrer& referrer) OVERRIDE;
  virtual void GenerateMHTML(
      const base::FilePath& file,
      const base::Callback<void(int64)>& callback)
          OVERRIDE;
  virtual bool IsActiveEntry(int32 page_id) OVERRIDE;

  virtual const std::string& GetContentsMimeType() const OVERRIDE;
  virtual bool WillNotifyDisconnection() const OVERRIDE;
  virtual void SetOverrideEncoding(const std::string& encoding) OVERRIDE;
  virtual void ResetOverrideEncoding() OVERRIDE;
  virtual RendererPreferences* GetMutableRendererPrefs() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void SystemDragEnded() OVERRIDE;
  virtual void UserGestureDone() OVERRIDE;
  virtual void SetClosedByUserGesture(bool value) OVERRIDE;
  virtual bool GetClosedByUserGesture() const OVERRIDE;
  virtual double GetZoomLevel() const OVERRIDE;
  virtual int GetZoomPercent(bool* enable_increment,
                             bool* enable_decrement) const OVERRIDE;
  virtual void ViewSource() OVERRIDE;
  virtual void ViewFrameSource(const GURL& url,
                               const PageState& page_state) OVERRIDE;
  virtual int GetMinimumZoomPercent() const OVERRIDE;
  virtual int GetMaximumZoomPercent() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() const OVERRIDE;
  virtual bool GotResponseToLockMouseRequest(bool allowed) OVERRIDE;
  virtual bool HasOpener() const OVERRIDE;
  virtual void DidChooseColorInColorChooser(SkColor color) OVERRIDE;
  virtual void DidEndColorChooser() OVERRIDE;
  virtual int DownloadImage(const GURL& url,
                            bool is_favicon,
                            uint32_t max_bitmap_size,
                            const ImageDownloadCallback& callback) OVERRIDE;
  virtual void SetZoomLevel(double level) OVERRIDE;
#if defined(OS_ANDROID)
  virtual base::android::ScopedJavaLocalRef<jobject> GetJavaWebContents()
      OVERRIDE;
#endif

  
  virtual WebContents* OpenURL(const OpenURLParams& params) OVERRIDE;

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  virtual bool OnMessageReceived(RenderFrameHost* render_frame_host,
                                 const IPC::Message& message) OVERRIDE;
  virtual void RenderFrameCreated(RenderFrameHost* render_frame_host) OVERRIDE;
  virtual void RenderFrameDeleted(RenderFrameHost* render_frame_host) OVERRIDE;

  
  virtual RenderViewHostDelegateView* GetDelegateView() OVERRIDE;
  virtual RenderViewHostDelegate::RendererManagement*
      GetRendererManagementDelegate() OVERRIDE;
  virtual bool OnMessageReceived(RenderViewHost* render_view_host,
                                 const IPC::Message& message) OVERRIDE;
  virtual const GURL& GetURL() const OVERRIDE;
  virtual const GURL& GetVisibleURL() const OVERRIDE;
  virtual const GURL& GetLastCommittedURL() const OVERRIDE;
  virtual WebContents* GetAsWebContents() OVERRIDE;
  virtual gfx::Rect GetRootWindowResizerRect() const OVERRIDE;
  virtual void RenderViewCreated(RenderViewHost* render_view_host) OVERRIDE;
  virtual void RenderViewReady(RenderViewHost* render_view_host) OVERRIDE;
  virtual void RenderViewTerminated(RenderViewHost* render_view_host,
                                    base::TerminationStatus status,
                                    int error_code) OVERRIDE;
  virtual void RenderViewDeleted(RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidRedirectProvisionalLoad(
      RenderViewHost* render_view_host,
      int32 page_id,
      const GURL& source_url,
      const GURL& target_url) OVERRIDE;
  virtual void DidFailProvisionalLoadWithError(
      RenderViewHost* render_view_host,
      const ViewHostMsg_DidFailProvisionalLoadWithError_Params& params)
          OVERRIDE;
  virtual void DidGetResourceResponseStart(
      const ResourceRequestDetails& details) OVERRIDE;
  virtual void DidGetRedirectForResourceRequest(
      const ResourceRedirectDetails& details) OVERRIDE;
  virtual void DidNavigate(
      RenderViewHost* render_view_host,
      const ViewHostMsg_FrameNavigate_Params& params) OVERRIDE;
  virtual void UpdateState(RenderViewHost* render_view_host,
                           int32 page_id,
                           const PageState& page_state) OVERRIDE;
  virtual void UpdateTitle(RenderViewHost* render_view_host,
                           int32 page_id,
                           const base::string16& title,
                           base::i18n::TextDirection title_direction) OVERRIDE;
  virtual void UpdateEncoding(RenderViewHost* render_view_host,
                              const std::string& encoding) OVERRIDE;
  virtual void UpdateTargetURL(int32 page_id, const GURL& url) OVERRIDE;
  virtual void Close(RenderViewHost* render_view_host) OVERRIDE;
  virtual void RequestMove(const gfx::Rect& new_bounds) OVERRIDE;
  virtual void SwappedOut(RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidStartLoading(RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidStopLoading(RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidCancelLoading() OVERRIDE;
  virtual void DidChangeLoadProgress(double progress) OVERRIDE;
  virtual void DidDisownOpener(RenderViewHost* rvh) OVERRIDE;
  virtual void DidAccessInitialDocument() OVERRIDE;
  virtual void DocumentAvailableInMainFrame(
      RenderViewHost* render_view_host) OVERRIDE;
  virtual void DocumentOnLoadCompletedInMainFrame(
      RenderViewHost* render_view_host,
      int32 page_id) OVERRIDE;
  virtual void RequestOpenURL(RenderViewHost* rvh,
                              const GURL& url,
                              const Referrer& referrer,
                              WindowOpenDisposition disposition,
                              int64 source_frame_id,
                              bool should_replace_current_entry,
                              bool user_gesture) OVERRIDE;
  virtual void RequestTransferURL(
      const GURL& url,
      const std::vector<GURL>& redirect_chain,
      const Referrer& referrer,
      PageTransition page_transition,
      WindowOpenDisposition disposition,
      int64 source_frame_id,
      const GlobalRequestID& transferred_global_request_id,
      bool should_replace_current_entry,
      bool user_gesture) OVERRIDE;
  virtual void RouteCloseEvent(RenderViewHost* rvh) OVERRIDE;
  virtual void RouteMessageEvent(
      RenderViewHost* rvh,
      const ViewMsg_PostMessage_Params& params) OVERRIDE;
  virtual void RunJavaScriptMessage(RenderViewHost* rvh,
                                    const base::string16& message,
                                    const base::string16& default_prompt,
                                    const GURL& frame_url,
                                    JavaScriptMessageType type,
                                    IPC::Message* reply_msg,
                                    bool* did_suppress_message) OVERRIDE;
  virtual void RunBeforeUnloadConfirm(RenderViewHost* rvh,
                                      const base::string16& message,
                                      bool is_reload,
                                      IPC::Message* reply_msg) OVERRIDE;
  virtual bool AddMessageToConsole(int32 level,
                                   const base::string16& message,
                                   int32 line_no,
                                   const base::string16& source_id) OVERRIDE;
  virtual RendererPreferences GetRendererPrefs(
      BrowserContext* browser_context) const OVERRIDE;
  virtual WebPreferences GetWebkitPrefs() OVERRIDE;
  virtual void OnUserGesture() OVERRIDE;
  virtual void OnIgnoredUIEvent() OVERRIDE;
  virtual void RendererUnresponsive(RenderViewHost* render_view_host,
                                    bool is_during_beforeunload,
                                    bool is_during_unload) OVERRIDE;
  virtual void RendererResponsive(RenderViewHost* render_view_host) OVERRIDE;
  virtual void LoadStateChanged(const GURL& url,
                                const net::LoadStateWithParam& load_state,
                                uint64 upload_position,
                                uint64 upload_size) OVERRIDE;
  virtual void WorkerCrashed() OVERRIDE;
  virtual void Activate() OVERRIDE;
  virtual void Deactivate() OVERRIDE;
  virtual void LostCapture() OVERRIDE;
  virtual void HandleMouseDown() OVERRIDE;
  virtual void HandleMouseUp() OVERRIDE;
  virtual void HandlePointerActivate() OVERRIDE;
  virtual void HandleGestureBegin() OVERRIDE;
  virtual void HandleGestureEnd() OVERRIDE;
  virtual void RunFileChooser(
      RenderViewHost* render_view_host,
      const FileChooserParams& params) OVERRIDE;
  virtual void ToggleFullscreenMode(bool enter_fullscreen) OVERRIDE;
  virtual bool IsFullscreenForCurrentTab() const OVERRIDE;
  virtual void UpdatePreferredSize(const gfx::Size& pref_size) OVERRIDE;
  virtual void ResizeDueToAutoResize(const gfx::Size& new_size) OVERRIDE;
  virtual void RequestToLockMouse(bool user_gesture,
                                  bool last_unlocked_by_target) OVERRIDE;
  virtual void LostMouseLock() OVERRIDE;
  virtual void CreateNewWindow(
      int render_process_id,
      int route_id,
      int main_frame_route_id,
      const ViewHostMsg_CreateWindow_Params& params,
      SessionStorageNamespace* session_storage_namespace) OVERRIDE;
  virtual void CreateNewWidget(int render_process_id,
                               int route_id,
                               blink::WebPopupType popup_type) OVERRIDE;
  virtual void CreateNewFullscreenWidget(int render_process_id,
                                         int route_id) OVERRIDE;
  virtual void ShowCreatedWindow(int route_id,
                                 WindowOpenDisposition disposition,
                                 const gfx::Rect& initial_pos,
                                 bool user_gesture) OVERRIDE;
  virtual void ShowCreatedWidget(int route_id,
                                 const gfx::Rect& initial_pos) OVERRIDE;
  virtual void ShowCreatedFullscreenWidget(int route_id) OVERRIDE;
  virtual void ShowContextMenu(const ContextMenuParams& params) OVERRIDE;
  virtual void RequestMediaAccessPermission(
      const MediaStreamRequest& request,
      const MediaResponseCallback& callback) OVERRIDE;
  virtual SessionStorageNamespace* GetSessionStorageNamespace(
      SiteInstance* instance) OVERRIDE;
  virtual FrameTree* GetFrameTree() OVERRIDE;

  

  virtual void DidStartProvisionalLoad(
      RenderFrameHostImpl* render_frame_host,
      int64 frame_id,
      int64 parent_frame_id,
      bool is_main_frame,
      const GURL& validated_url,
      bool is_error_page,
      bool is_iframe_srcdoc) OVERRIDE;
  virtual void NotifyChangedNavigationState(
      InvalidateTypes changed_flags) OVERRIDE;

  

  virtual void RenderWidgetDeleted(
      RenderWidgetHostImpl* render_widget_host) OVERRIDE;
  virtual bool PreHandleKeyboardEvent(
      const NativeWebKeyboardEvent& event,
      bool* is_keyboard_shortcut) OVERRIDE;
  virtual void HandleKeyboardEvent(
      const NativeWebKeyboardEvent& event) OVERRIDE;
  virtual bool PreHandleWheelEvent(
      const blink::WebMouseWheelEvent& event) OVERRIDE;
  virtual void DidSendScreenRects(RenderWidgetHostImpl* rwh) OVERRIDE;
#if defined(OS_WIN) && defined(USE_AURA)
  virtual gfx::NativeViewAccessible GetParentNativeViewAccessible() OVERRIDE;
#endif

  

  virtual bool CreateRenderViewForRenderManager(
      RenderViewHost* render_view_host, int opener_route_id) OVERRIDE;
  virtual void BeforeUnloadFiredFromRenderManager(
      bool proceed, const base::TimeTicks& proceed_time,
      bool* proceed_to_fire_unload) OVERRIDE;
  virtual void RenderProcessGoneFromRenderManager(
      RenderViewHost* render_view_host) OVERRIDE;
  virtual void UpdateRenderViewSizeForRenderManager() OVERRIDE;
  virtual void CancelModalDialogsForRenderManager() OVERRIDE;
  virtual void NotifySwappedFromRenderManager(
      RenderViewHost* old_host, RenderViewHost* new_host) OVERRIDE;
  virtual int CreateOpenerRenderViewsForRenderManager(
      SiteInstance* instance) OVERRIDE;
  virtual NavigationControllerImpl&
      GetControllerForRenderManager() OVERRIDE;
  virtual WebUIImpl* CreateWebUIForRenderManager(const GURL& url) OVERRIDE;
  virtual NavigationEntry*
      GetLastCommittedNavigationEntryForRenderManager() OVERRIDE;
  virtual bool FocusLocationBarByDefault() OVERRIDE;
  virtual void SetFocusToLocationBar(bool select_all) OVERRIDE;
  virtual void CreateViewAndSetSizeForRVH(RenderViewHost* rvh) OVERRIDE;
  virtual bool IsHidden() OVERRIDE;

  

  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  

  virtual WebContents* GetWebContents() OVERRIDE;
  virtual void NotifyNavigationEntryCommitted(
      const LoadCommittedDetails& load_details) OVERRIDE;

  
  virtual void NotifyBeforeFormRepostWarningShow() OVERRIDE;

  
  virtual void ActivateAndShowRepostFormWarningDialog() OVERRIDE;

  
  
  virtual void UpdateMaxPageID(int32 page_id) OVERRIDE;

  
  
  virtual void UpdateMaxPageIDForSiteInstance(SiteInstance* site_instance,
                                              int32 page_id) OVERRIDE;

  
  
  
  virtual void CopyMaxPageIDsFrom(WebContents* web_contents) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  virtual bool NavigateToPendingEntry(
      NavigationController::ReloadType reload_type) OVERRIDE;

  
  
  
  
  virtual void SetHistoryLengthAndPrune(
      const SiteInstance* site_instance,
      int merge_history_length,
      int32 minimum_page_id) OVERRIDE;

  
  virtual void RenderViewForInterstitialPageCreated(
      RenderViewHost* render_view_host) OVERRIDE;

  
  
  virtual void AttachInterstitialPage(
      InterstitialPageImpl* interstitial_page) OVERRIDE;

  
  virtual void DetachInterstitialPage() OVERRIDE;

  
  
  
  virtual void SetIsLoading(RenderViewHost* render_view_host,
                            bool is_loading,
                            LoadNotificationDetails* details) OVERRIDE;

  typedef base::Callback<void(WebContents*)> CreatedCallback;

 private:
  friend class NavigationControllerImpl;
  friend class TestNavigationObserver;
  friend class WebContentsObserver;
  friend class WebContents;  

  FRIEND_TEST_ALL_PREFIXES(WebContentsImplTest, NoJSMessageOnInterstitials);
  FRIEND_TEST_ALL_PREFIXES(WebContentsImplTest, UpdateTitle);
  FRIEND_TEST_ALL_PREFIXES(WebContentsImplTest, FindOpenerRVHWhenPending);
  FRIEND_TEST_ALL_PREFIXES(WebContentsImplTest,
                           CrossSiteCantPreemptAfterUnload);
  FRIEND_TEST_ALL_PREFIXES(WebContentsImplTest, PendingContents);
  FRIEND_TEST_ALL_PREFIXES(WebContentsImplTest, FrameTreeShape);
  FRIEND_TEST_ALL_PREFIXES(FormStructureBrowserTest, HTMLFiles);
  FRIEND_TEST_ALL_PREFIXES(NavigationControllerTest, HistoryNavigate);
  FRIEND_TEST_ALL_PREFIXES(RenderFrameHostManagerTest, PageDoesBackAndReload);

  
  friend class InterstitialPageImpl;

  
  friend class TestWebContents;

  class DestructionObserver;

  
  WebContentsImpl(BrowserContext* browser_context,
                  WebContentsImpl* opener);

  
  
  
  void AddObserver(WebContentsObserver* observer);
  void RemoveObserver(WebContentsObserver* observer);

  
  void OnWebContentsDestroyed(WebContentsImpl* web_contents);

  
  
  void AddDestructionObserver(WebContentsImpl* web_contents);

  
  
  void RemoveDestructionObserver(WebContentsImpl* web_contents);

  
  void OnDialogClosed(RenderViewHost* rvh,
                      IPC::Message* reply_msg,
                      bool success,
                      const base::string16& user_input);

  
  
  void OnPpapiBrokerPermissionResult(int routing_id, bool result);

  bool OnMessageReceived(RenderViewHost* render_view_host,
                         RenderFrameHost* render_frame_host,
                         const IPC::Message& message);

  
  void OnDidLoadResourceFromMemoryCache(const GURL& url,
                                        const std::string& security_info,
                                        const std::string& http_request,
                                        const std::string& mime_type,
                                        ResourceType::Type resource_type);
  void OnDidDisplayInsecureContent();
  void OnDidRunInsecureContent(const std::string& security_origin,
                               const GURL& target_url);
  void OnDocumentLoadedInFrame(int64 frame_id);
  void OnDidFinishLoad(int64 frame_id,
                       const GURL& url,
                       bool is_main_frame);
  void OnDidFailLoadWithError(int64 frame_id,
                              const GURL& url,
                              bool is_main_frame,
                              int error_code,
                              const base::string16& error_description);
  void OnGoToEntryAtOffset(int offset);
  void OnUpdateZoomLimits(int minimum_percent,
                          int maximum_percent,
                          bool remember);
  void OnEnumerateDirectory(int request_id, const base::FilePath& path);
  void OnJSOutOfMemory();

  void OnRegisterProtocolHandler(const std::string& protocol,
                                 const GURL& url,
                                 const base::string16& title,
                                 bool user_gesture);
  void OnFindReply(int request_id,
                   int number_of_matches,
                   const gfx::Rect& selection_rect,
                   int active_match_ordinal,
                   bool final_update);
#if defined(OS_ANDROID)
  void OnFindMatchRectsReply(int version,
                             const std::vector<gfx::RectF>& rects,
                             const gfx::RectF& active_rect);

  void OnOpenDateTimeDialog(
      const ViewHostMsg_DateTimeDialogValue_Params& value);
  void OnJavaBridgeGetChannelHandle(IPC::Message* reply_msg);
#endif
  void OnPepperPluginHung(int plugin_child_id,
                          const base::FilePath& path,
                          bool is_hung);
  void OnPluginCrashed(const base::FilePath& plugin_path,
                       base::ProcessId plugin_pid);
  void OnAppCacheAccessed(const GURL& manifest_url, bool blocked_by_policy);
  void OnOpenColorChooser(int color_chooser_id,
                          SkColor color,
                          const std::vector<ColorSuggestion>& suggestions);
  void OnEndColorChooser(int color_chooser_id);
  void OnSetSelectedColorInColorChooser(int color_chooser_id, SkColor color);
  void OnWebUISend(const GURL& source_url,
                   const std::string& name,
                   const base::ListValue& args);
  void OnRequestPpapiBrokerPermission(int routing_id,
                                      const GURL& url,
                                      const base::FilePath& plugin_path);
  void OnBrowserPluginMessage(const IPC::Message& message);
  void OnDidDownloadImage(int id,
                          int http_status_code,
                          const GURL& image_url,
                          const std::vector<SkBitmap>& bitmaps,
                          const std::vector<gfx::Size>& original_bitmap_sizes);
  void OnUpdateFaviconURL(int32 page_id,
                          const std::vector<FaviconURL>& candidates);
  void OnFirstVisuallyNonEmptyPaint(int32 page_id);
  void OnMediaNotification(int64 player_cookie,
                           bool has_video,
                           bool has_audio,
                           bool is_playing);
  void OnShowValidationMessage(const gfx::Rect& anchor_in_root_view,
                               const string16& main_text,
                               const string16& sub_text);
  void OnHideValidationMessage();
  void OnMoveValidationMessage(const gfx::Rect& anchor_in_root_view);


  
  
  
  void SetNotWaitingForResponse() { waiting_for_response_ = false; }

  
  
  

  
  
  
  
  void DidNavigateMainFramePostCommit(
      const LoadCommittedDetails& details,
      const ViewHostMsg_FrameNavigate_Params& params);
  void DidNavigateAnyFramePostCommit(
      RenderViewHost* render_view_host,
      const LoadCommittedDetails& details,
      const ViewHostMsg_FrameNavigate_Params& params);

  
  
  bool ShouldAssignSiteForURL(const GURL& url);

  
  
  
  
  void UpdateMaxPageIDIfNecessary(RenderViewHost* rvh);

  
  
  
  
  
  
  
  
  bool UpdateTitleForEntry(NavigationEntryImpl* entry,
                           const base::string16& title);

  
  
  
  bool NavigateToEntry(const NavigationEntryImpl& entry,
                       NavigationController::ReloadType reload_type);

  
  
  
  
  int CreateOpenerRenderViews(SiteInstance* instance);

  
  void CreateNewWidget(int render_process_id,
                       int route_id,
                       bool is_fullscreen,
                       blink::WebPopupType popup_type);

  
  void ShowCreatedWidget(int route_id,
                         bool is_fullscreen,
                         const gfx::Rect& initial_pos);

  
  
  RenderWidgetHostView* GetCreatedWidget(int route_id);

  
  
  
  WebContentsImpl* GetCreatedWindow(int route_id);

  
  
  
  
  
  RenderWidgetHostViewPort* GetRenderWidgetHostViewPort() const;

  

  
  void NotifySwapped(RenderViewHost* old_host, RenderViewHost* new_host);
  void NotifyDisconnected();

  void SetEncoding(const std::string& encoding);

  
  
  RenderFrameHostManager* GetRenderManager() const;

  RenderViewHostImpl* GetRenderViewHostImpl();

  
  void RemoveBrowserPluginEmbedder();

  
  void ClearPowerSaveBlockers(RenderViewHost* render_view_host);

  
  void ClearAllPowerSaveBlockers();

  
  gfx::Size GetSizeForNewRenderView() const;

  void OnFrameRemoved(RenderViewHostImpl* render_view_host, int64 frame_id);

  
  
  static void AddCreatedCallback(const CreatedCallback& callback);
  static void RemoveCreatedCallback(const CreatedCallback& callback);

  

  
  WebContentsDelegate* delegate_;

  
  NavigationControllerImpl controller_;

  
  scoped_ptr<WebContentsViewPort> view_;

  
  
  
  RenderViewHostDelegateView* render_view_host_delegate_view_;

  
  
  typedef std::map<int, WebContentsImpl*> PendingContents;
  PendingContents pending_contents_;

  
  
  typedef std::map<int, RenderWidgetHostView*> PendingWidgetViews;
  PendingWidgetViews pending_widget_views_;

  typedef std::map<WebContentsImpl*, DestructionObserver*> DestructionObservers;
  DestructionObservers destruction_observers_;

  
  
  
  
  ObserverList<WebContentsObserver> observers_;

  
  
  WebContentsImpl* opener_;

#if defined(OS_WIN) && defined(USE_AURA)
  gfx::NativeViewAccessible accessible_parent_;
#endif

  

  
  
  
  typedef std::map<RenderViewHost*, std::map<int64, PowerSaveBlocker*> >
      PowerSaveBlockerMap;
  PowerSaveBlockerMap power_save_blockers_;

  
  FrameTree frame_tree_;

#if defined(OS_ANDROID)
  
  
  scoped_ptr<JavaBridgeDispatcherHostManager>
      java_bridge_dispatcher_host_manager_;
#endif

  
  scoped_refptr<SavePackage> save_package_;

  

  
  bool is_loading_;

  
  base::TerminationStatus crashed_status_;
  int crashed_error_code_;

  
  
  
  bool waiting_for_response_;

  
  
  
  std::map<int32, int32> max_page_ids_;

  
  base::TimeTicks current_load_start_;

  
  net::LoadStateWithParam load_state_;
  base::string16 load_state_host_;
  
  
  uint64 upload_size_;
  uint64 upload_position_;

  

  
  base::string16 page_title_when_no_navigation_entry_;

  
  
  std::string contents_mime_type_;

  
  std::string encoding_;

  
  bool displayed_insecure_content_;

  

  
  
  
  int capturer_count_;

  
  bool should_normally_be_visible_;

  
  bool is_being_destroyed_;

  
  
  
  
  bool notify_disconnection_;

  
  
  JavaScriptDialogManager* dialog_manager_;

  
  
  
  bool is_showing_before_unload_dialog_;

  
  RendererPreferences renderer_preferences_;

  
  base::TimeTicks last_selected_time_;

  
  bool closed_by_user_gesture_;

  
  int minimum_zoom_percent_;
  int maximum_zoom_percent_;
  
  
  
  bool temporary_zoom_settings_;

  
  gfx::Size preferred_size_;

#if defined(OS_ANDROID)
  
  
  scoped_ptr<DateTimeChooserAndroid> date_time_chooser_;
#endif

  
  scoped_ptr<ColorChooser> color_chooser_;

  
  
  
  
  
  
  
  int color_chooser_identifier_;

  
  
  scoped_ptr<BrowserPluginEmbedder> browser_plugin_embedder_;
  
  
  scoped_ptr<BrowserPluginGuest> browser_plugin_guest_;

  
  
  NotificationRegistrar registrar_;

  
  
  
  RenderViewHost* render_view_message_source_;

  
  
  std::set<RenderWidgetHostImpl*> created_widgets_;

  
  int fullscreen_widget_routing_id_;

  
  typedef std::map<int, ImageDownloadCallback> ImageDownloadMap;
  ImageDownloadMap image_download_map_;

  DISALLOW_COPY_AND_ASSIGN(WebContentsImpl);
};

}  

#endif  
