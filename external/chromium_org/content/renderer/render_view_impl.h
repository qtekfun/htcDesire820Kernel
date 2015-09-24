// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_RENDER_VIEW_IMPL_H_
#define CONTENT_RENDERER_RENDER_VIEW_IMPL_H_

#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/id_map.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/process/process.h"
#include "base/strings/string16.h"
#include "base/timer/timer.h"
#include "build/build_config.h"
#include "cc/input/top_controls_state.h"
#include "content/common/content_export.h"
#include "content/common/drag_event_source_info.h"
#include "content/common/edit_command.h"
#include "content/common/gpu/client/webgraphicscontext3d_command_buffer_impl.h"
#include "content/common/navigation_gesture.h"
#include "content/common/view_message_enums.h"
#include "content/public/common/javascript_message_type.h"
#include "content/public/common/page_zoom.h"
#include "content/public/common/referrer.h"
#include "content/public/common/renderer_preferences.h"
#include "content/public/common/stop_find_action.h"
#include "content/public/common/top_controls_state.h"
#include "content/public/renderer/render_view.h"
#include "content/renderer/media/webmediaplayer_delegate.h"
#include "content/renderer/mouse_lock_dispatcher.h"
#include "content/renderer/render_frame_impl.h"
#include "content/renderer/render_widget.h"
#include "content/renderer/renderer_date_time_picker.h"
#include "content/renderer/renderer_webcookiejar_impl.h"
#include "content/renderer/stats_collection_observer.h"
#include "ipc/ipc_platform_file.h"
#include "third_party/WebKit/public/platform/WebGraphicsContext3D.h"
#include "third_party/WebKit/public/web/WebAXObject.h"
#include "third_party/WebKit/public/web/WebConsoleMessage.h"
#include "third_party/WebKit/public/web/WebDataSource.h"
#include "third_party/WebKit/public/web/WebFrameClient.h"
#include "third_party/WebKit/public/web/WebHistoryItem.h"
#include "third_party/WebKit/public/web/WebIconURL.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"
#include "third_party/WebKit/public/web/WebNavigationType.h"
#include "third_party/WebKit/public/web/WebNode.h"
#include "third_party/WebKit/public/web/WebPageSerializerClient.h"
#include "third_party/WebKit/public/web/WebPageVisibilityState.h"
#include "third_party/WebKit/public/web/WebSecurityOrigin.h"
#include "third_party/WebKit/public/web/WebViewClient.h"
#include "ui/base/ui_base_types.h"
#include "ui/surface/transport_dib.h"
#include "webkit/common/webpreferences.h"

#if defined(OS_ANDROID)
#include "content/renderer/android/content_detector.h"
#include "third_party/WebKit/public/web/WebContentDetectionResult.h"
#endif

#if defined(COMPILER_MSVC)
#pragma warning(disable: 4250)
#endif

class CommandLine;
class PepperDeviceTest;
class SkBitmap;
struct PP_NetAddress_Private;
struct ViewMsg_Navigate_Params;
struct ViewMsg_PostMessage_Params;
struct ViewMsg_StopFinding_Params;

namespace ui {
struct SelectedFileInfo;
}  

namespace blink {
class WebApplicationCacheHost;
class WebApplicationCacheHostClient;
class WebDOMMessageEvent;
class WebDataSource;
class WebDateTimeChooserCompletion;
class WebDragData;
class WebGeolocationClient;
class WebGestureEvent;
class WebIconURL;
class WebImage;
class WebPeerConnection00Handler;
class WebPeerConnection00HandlerClient;
class WebMediaPlayer;
class WebMediaPlayerClient;
class WebMouseEvent;
class WebPeerConnectionHandler;
class WebPeerConnectionHandlerClient;
class WebSocketStreamHandle;
class WebSpeechInputController;
class WebSpeechInputListener;
class WebSpeechRecognizer;
class WebStorageNamespace;
class WebTouchEvent;
class WebURLRequest;
class WebUserMediaClient;
struct WebActiveWheelFlingParameters;
struct WebDateTimeChooserParams;
struct WebFileChooserParams;
struct WebFindOptions;
struct WebMediaPlayerAction;
struct WebPluginAction;
struct WebPoint;
struct WebWindowFeatures;

#if defined(OS_ANDROID)
class WebHitTestResult;
#endif
}

namespace webkit_glue {
class WebURLResponseExtraDataImpl;
}

namespace content {
class BrowserPluginManager;
class DeviceOrientationDispatcher;
class DevToolsAgent;
class DocumentState;
class DomAutomationController;
class ExternalPopupMenu;
class FaviconHelper;
class GeolocationDispatcher;
class ImageResourceFetcher;
class InputTagSpeechDispatcher;
class JavaBridgeDispatcher;
class LoadProgressTracker;
class MIDIDispatcher;
class MediaStreamClient;
class MediaStreamDispatcher;
class MouseLockDispatcher;
class NavigationState;
class NotificationProvider;
class PepperPluginInstanceImpl;
class RenderViewObserver;
class RenderViewTest;
class RendererAccessibility;
class RendererDateTimePicker;
class RendererWebColorChooserImpl;
class SpeechRecognitionDispatcher;
class WebPluginDelegateProxy;
struct CustomContextMenuContext;
struct DropData;
struct FaviconURL;
struct FileChooserParams;
struct RenderViewImplParams;

#if defined(OS_ANDROID)
class RendererMediaPlayerManager;
class WebMediaPlayerProxyAndroid;
#endif

class CONTENT_EXPORT RenderViewImpl
    : public RenderWidget,
      NON_EXPORTED_BASE(public blink::WebViewClient),
      NON_EXPORTED_BASE(public blink::WebFrameClient),
      NON_EXPORTED_BASE(public blink::WebPageSerializerClient),
      public RenderView,
      NON_EXPORTED_BASE(public WebMediaPlayerDelegate),
      public base::SupportsWeakPtr<RenderViewImpl> {
 public:
  
  
  static RenderViewImpl* Create(
      int32 opener_id,
      const RendererPreferences& renderer_prefs,
      const WebPreferences& webkit_prefs,
      int32 routing_id,
      int32 main_frame_routing_id,
      int32 surface_id,
      int64 session_storage_namespace_id,
      const base::string16& frame_name,
      bool is_renderer_created,
      bool swapped_out,
      bool hidden,
      int32 next_page_id,
      const blink::WebScreenInfo& screen_info,
      AccessibilityMode accessibility_mode,
      bool allow_partial_swap);

  
  
  static void InstallCreateHook(
      RenderViewImpl* (*create_render_view_impl)(RenderViewImplParams*));

  
  static RenderViewImpl* FromWebView(blink::WebView* webview);

  
  static RenderViewImpl* FromRoutingID(int routing_id);

  
  blink::WebView* webview() const;

  int history_list_offset() const { return history_list_offset_; }

  const WebPreferences& webkit_preferences() const {
    return webkit_preferences_;
  }

  const RendererPreferences& renderer_preferences() const {
    return renderer_preferences_;
  }

  void set_send_content_state_immediately(bool value) {
    send_content_state_immediately_ = value;
  }

  RenderFrameImpl* main_render_frame() { return main_render_frame_.get(); }

  MediaStreamDispatcher* media_stream_dispatcher() {
    return media_stream_dispatcher_;
  }

  RendererAccessibility* renderer_accessibility() {
    return renderer_accessibility_;
  }

  MouseLockDispatcher* mouse_lock_dispatcher() {
    return mouse_lock_dispatcher_;
  }

  RendererWebCookieJarImpl* cookie_jar() { return &cookie_jar_; }

  
  BrowserPluginManager* GetBrowserPluginManager();

  
  void AddObserver(RenderViewObserver* observer);
  void RemoveObserver(RenderViewObserver* observer);

  
  
  StatsCollectionObserver* GetStatsCollectionObserver() {
    return stats_collection_observer_.get();
  }

  
  
  
  
  
  
  bool ScheduleFileChooser(const FileChooserParams& params,
                           blink::WebFileChooserCompletion* completion);

  void LoadNavigationErrorPage(
      blink::WebFrame* frame,
      const blink::WebURLRequest& failed_request,
      const blink::WebURLError& error,
      const std::string& html,
      bool replace);

#if defined(OS_ANDROID)
  void DismissDateTimeDialog();
#endif

  

#if defined(ENABLE_PLUGINS)
  PepperPluginInstanceImpl* focused_pepper_plugin() {
    return focused_pepper_plugin_;
  }
  void set_focused_pepper_plugin(PepperPluginInstanceImpl* plugin) {
    focused_pepper_plugin_ = plugin;
  }
  PepperPluginInstanceImpl* pepper_last_mouse_event_target() {
    return pepper_last_mouse_event_target_;
  }
  void set_pepper_last_mouse_event_target(PepperPluginInstanceImpl* plugin) {
    pepper_last_mouse_event_target_ = plugin;
  }

#if defined(OS_MACOSX) || defined(OS_WIN)
  
  void PluginFocusChanged(bool focused, int plugin_id);
#endif

#if defined(OS_MACOSX)
  
  void StartPluginIme();
#endif

  void RegisterPluginDelegate(WebPluginDelegateProxy* delegate);
  void UnregisterPluginDelegate(WebPluginDelegateProxy* delegate);
#endif  

  void TransferActiveWheelFlingAnimation(
      const blink::WebActiveWheelFlingParameters& params);

  
  
  
  bool HasIMETextFocus();

  
  typedef base::Callback<void(
      const gfx::Size&, const std::vector<unsigned char>&)>
      WindowSnapshotCallback;

  void GetWindowSnapshot(const WindowSnapshotCallback& callback);

  
  
  void SyncNavigationState();

  
  
  blink::WebMediaPlayer* CreateMediaPlayer(
      RenderFrame* render_frame,
      blink::WebFrame* frame,
      const blink::WebURL& url,
      blink::WebMediaPlayerClient* client);
  
  
  int ShowContextMenu(ContextMenuClient* client,
                      const ContextMenuParams& params);
  void CancelContextMenu(int request_id);

  
  
  
  unsigned GetLocalSessionHistoryLengthForTesting() const;

  
  
  
  void SetFocusAndActivateForTesting(bool enable);

  
  void SetDeviceScaleFactorForTesting(float factor);

  
  void ForceResizeForTesting(const gfx::Size& new_size);

  void UseSynchronousResizeModeForTesting(bool enable);

  
  void EnableAutoResizeForTesting(const gfx::Size& min_size,
                                  const gfx::Size& max_size);
  void DisableAutoResizeForTesting(const gfx::Size& new_size);

  
  
  void SetMediaStreamClientForTesting(MediaStreamClient* media_stream_client);

  

  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

  

  
  virtual void didFocus();
  virtual void didBlur();
  virtual void show(blink::WebNavigationPolicy policy);
  virtual void runModal();
  virtual bool enterFullScreen();
  virtual void exitFullScreen();
  virtual bool requestPointerLock();
  virtual void requestPointerUnlock();
  virtual bool isPointerLocked();
  virtual void didActivateCompositor(int input_handler_identifier);
  virtual void didHandleGestureEvent(const blink::WebGestureEvent& event,
                                     bool event_cancelled) OVERRIDE;
  virtual void initializeLayerTreeView() OVERRIDE;

  

  virtual blink::WebView* createView(
      blink::WebFrame* creator,
      const blink::WebURLRequest& request,
      const blink::WebWindowFeatures& features,
      const blink::WebString& frame_name,
      blink::WebNavigationPolicy policy,
      bool suppress_opener);
  virtual blink::WebWidget* createPopupMenu(blink::WebPopupType popup_type);
  virtual blink::WebExternalPopupMenu* createExternalPopupMenu(
      const blink::WebPopupMenuInfo& popup_menu_info,
      blink::WebExternalPopupMenuClient* popup_menu_client);
  virtual blink::WebStorageNamespace* createSessionStorageNamespace();
  virtual bool shouldReportDetailedMessageForSource(
      const blink::WebString& source);
  virtual void didAddMessageToConsole(
      const blink::WebConsoleMessage& message,
      const blink::WebString& source_name,
      unsigned source_line,
      const blink::WebString& stack_trace);
  virtual void printPage(blink::WebFrame* frame);
  virtual blink::WebNotificationPresenter* notificationPresenter();
  virtual bool enumerateChosenDirectory(
      const blink::WebString& path,
      blink::WebFileChooserCompletion* chooser_completion);
  virtual void initializeHelperPluginWebFrame(blink::WebHelperPlugin*);
  virtual void didStartLoading();
  virtual void didStopLoading();
  virtual void didChangeLoadProgress(blink::WebFrame* frame,
                                     double load_progress);
  virtual void didCancelCompositionOnSelectionChange();
  virtual void didChangeSelection(bool is_selection_empty);
  virtual void didExecuteCommand(const blink::WebString& command_name);
  virtual bool handleCurrentKeyboardEvent();
  virtual blink::WebColorChooser* createColorChooser(
      blink::WebColorChooserClient*,
      const blink::WebColor& initial_color,
      const blink::WebVector<blink::WebColorSuggestion>& suggestions);
  virtual bool runFileChooser(
      const blink::WebFileChooserParams& params,
      blink::WebFileChooserCompletion* chooser_completion);
  virtual void runModalAlertDialog(blink::WebFrame* frame,
                                   const blink::WebString& message);
  virtual bool runModalConfirmDialog(blink::WebFrame* frame,
                                     const blink::WebString& message);
  virtual bool runModalPromptDialog(blink::WebFrame* frame,
                                    const blink::WebString& message,
                                    const blink::WebString& default_value,
                                    blink::WebString* actual_value);
  virtual bool runModalBeforeUnloadDialog(blink::WebFrame* frame,
                                          bool is_reload,
                                          const blink::WebString& message);
  virtual void showValidationMessage(const blink::WebRect& anchor_in_root_view,
                                     const blink::WebString& main_text,
                                     const blink::WebString& sub_text,
                                     blink::WebTextDirection hint) OVERRIDE;
  virtual void hideValidationMessage() OVERRIDE;
  virtual void moveValidationMessage(
      const blink::WebRect& anchor_in_root_view) OVERRIDE;

  
  virtual bool runModalBeforeUnloadDialog(blink::WebFrame* frame,
                                          const blink::WebString& message);
  virtual void showContextMenu(blink::WebFrame* frame,
                               const blink::WebContextMenuData& data);
  virtual void clearContextMenu();
  virtual void setStatusText(const blink::WebString& text);
  virtual void setMouseOverURL(const blink::WebURL& url);
  virtual void setKeyboardFocusURL(const blink::WebURL& url);
  virtual void startDragging(blink::WebFrame* frame,
                             const blink::WebDragData& data,
                             blink::WebDragOperationsMask mask,
                             const blink::WebImage& image,
                             const blink::WebPoint& imageOffset);
  virtual bool acceptsLoadDrops();
  virtual void focusNext();
  virtual void focusPrevious();
  virtual void focusedNodeChanged(const blink::WebNode& node);
  virtual void numberOfWheelEventHandlersChanged(unsigned num_handlers);
  virtual void didUpdateLayout();
#if defined(OS_ANDROID)
  virtual bool didTapMultipleTargets(
      const blink::WebGestureEvent& event,
      const blink::WebVector<blink::WebRect>& target_rects);
#endif
  virtual void navigateBackForwardSoon(int offset);
  virtual int historyBackListCount();
  virtual int historyForwardListCount();
  virtual void postAccessibilityEvent(
      const blink::WebAXObject& obj, blink::WebAXEvent event);
  virtual void didUpdateInspectorSetting(const blink::WebString& key,
                                         const blink::WebString& value);
  virtual blink::WebGeolocationClient* geolocationClient();
  virtual blink::WebSpeechInputController* speechInputController(
      blink::WebSpeechInputListener* listener);
  virtual blink::WebSpeechRecognizer* speechRecognizer();
  virtual void zoomLimitsChanged(double minimum_level, double maximum_level);
  virtual void zoomLevelChanged();
  virtual double zoomLevelToZoomFactor(double zoom_level) const;
  virtual double zoomFactorToZoomLevel(double factor) const;
  virtual void registerProtocolHandler(const blink::WebString& scheme,
                                       const blink::WebString& base_url,
                                       const blink::WebString& url,
                                       const blink::WebString& title);
  virtual blink::WebPageVisibilityState visibilityState() const;
  virtual blink::WebUserMediaClient* userMediaClient();
  virtual blink::WebMIDIClient* webMIDIClient();
  virtual void draggableRegionsChanged();

#if defined(OS_ANDROID)
  virtual void scheduleContentIntent(const blink::WebURL& intent);
  virtual void cancelScheduledContentIntents();
  virtual blink::WebContentDetectionResult detectContentAround(
      const blink::WebHitTestResult& touch_hit);

  
  
  virtual bool openDateTimeChooser(const blink::WebDateTimeChooserParams&,
                                   blink::WebDateTimeChooserCompletion*);
  virtual void didScrollWithKeyboard(const blink::WebSize& delta);
#endif

  

  virtual blink::WebMediaPlayer* createMediaPlayer(
      blink::WebFrame* frame,
      const blink::WebURL& url,
      blink::WebMediaPlayerClient* client);
  virtual blink::WebCookieJar* cookieJar(blink::WebFrame* frame);
  virtual void didAccessInitialDocument(blink::WebFrame* frame);
  virtual void didDisownOpener(blink::WebFrame* frame);
  virtual void frameDetached(blink::WebFrame* frame);
  virtual void willClose(blink::WebFrame* frame);
  virtual void didMatchCSS(
      blink::WebFrame* frame,
      const blink::WebVector<blink::WebString>& newly_matching_selectors,
      const blink::WebVector<blink::WebString>& stopped_matching_selectors);

  
  virtual blink::WebNavigationPolicy decidePolicyForNavigation(
      blink::WebFrame* frame,
      blink::WebDataSource::ExtraData* extraData,
      const blink::WebURLRequest& request,
      blink::WebNavigationType type,
      blink::WebNavigationPolicy default_policy,
      bool is_redirect);
  
  virtual blink::WebNavigationPolicy decidePolicyForNavigation(
      blink::WebFrame* frame,
      const blink::WebURLRequest& request,
      blink::WebNavigationType type,
      blink::WebNavigationPolicy default_policy,
      bool is_redirect);
  virtual void willSendSubmitEvent(blink::WebFrame* frame,
                                   const blink::WebFormElement& form);
  virtual void willSubmitForm(blink::WebFrame* frame,
                              const blink::WebFormElement& form);
  virtual void didCreateDataSource(blink::WebFrame* frame,
                                   blink::WebDataSource* datasource);
  virtual void didStartProvisionalLoad(blink::WebFrame* frame);
  virtual void didReceiveServerRedirectForProvisionalLoad(
      blink::WebFrame* frame);
  virtual void didFailProvisionalLoad(blink::WebFrame* frame,
                                      const blink::WebURLError& error);
  virtual void didCommitProvisionalLoad(blink::WebFrame* frame,
                                        bool is_new_navigation);
  virtual void didClearWindowObject(blink::WebFrame* frame);
  virtual void didCreateDocumentElement(blink::WebFrame* frame);
  virtual void didReceiveTitle(blink::WebFrame* frame,
                               const blink::WebString& title,
                               blink::WebTextDirection direction);
  virtual void didChangeIcon(blink::WebFrame*,
                             blink::WebIconURL::Type);
  virtual void didFinishDocumentLoad(blink::WebFrame* frame);
  virtual void didHandleOnloadEvents(blink::WebFrame* frame);
  virtual void didFailLoad(blink::WebFrame* frame,
                           const blink::WebURLError& error);
  virtual void didFinishLoad(blink::WebFrame* frame);
  virtual void didNavigateWithinPage(blink::WebFrame* frame,
                                     bool is_new_navigation);
  virtual void didUpdateCurrentHistoryItem(blink::WebFrame* frame);
  virtual void willSendRequest(blink::WebFrame* frame,
                               unsigned identifier,
                               blink::WebURLRequest& request,
                               const blink::WebURLResponse& redirect_response);
  virtual void didReceiveResponse(blink::WebFrame* frame,
                                  unsigned identifier,
                                  const blink::WebURLResponse& response);
  virtual void didFinishResourceLoad(blink::WebFrame* frame,
                                     unsigned identifier);
  virtual void didLoadResourceFromMemoryCache(
      blink::WebFrame* frame,
      const blink::WebURLRequest& request,
      const blink::WebURLResponse&);
  virtual void didDisplayInsecureContent(blink::WebFrame* frame);
  virtual void didRunInsecureContent(
      blink::WebFrame* frame,
      const blink::WebSecurityOrigin& origin,
      const blink::WebURL& target);
  virtual void didExhaustMemoryAvailableForScript(blink::WebFrame* frame);
  virtual void didCreateScriptContext(blink::WebFrame* frame,
                                      v8::Handle<v8::Context>,
                                      int extension_group,
                                      int world_id);
  virtual void willReleaseScriptContext(blink::WebFrame* frame,
                                        v8::Handle<v8::Context>,
                                        int world_id);
  virtual void didChangeScrollOffset(blink::WebFrame* frame);
  virtual void willInsertBody(blink::WebFrame* frame);
  virtual void didFirstVisuallyNonEmptyLayout(blink::WebFrame*);
  virtual void didChangeContentsSize(blink::WebFrame* frame,
                                     const blink::WebSize& size);
  virtual void reportFindInPageMatchCount(int request_id,
                                          int count,
                                          bool final_update);
  virtual void reportFindInPageSelection(int request_id,
                                         int active_match_ordinal,
                                         const blink::WebRect& sel);
  virtual void requestStorageQuota(
      blink::WebFrame* frame,
      blink::WebStorageQuotaType type,
      unsigned long long requested_size,
      blink::WebStorageQuotaCallbacks* callbacks);
  virtual void willOpenSocketStream(
      blink::WebSocketStreamHandle* handle);
  virtual void willStartUsingPeerConnectionHandler(blink::WebFrame* frame,
      blink::WebRTCPeerConnectionHandler* handler);
  virtual bool willCheckAndDispatchMessageEvent(
      blink::WebFrame* sourceFrame,
      blink::WebFrame* targetFrame,
      blink::WebSecurityOrigin targetOrigin,
      blink::WebDOMMessageEvent event);
  virtual blink::WebString acceptLanguages();
  virtual blink::WebString userAgentOverride(
      blink::WebFrame* frame,
      const blink::WebURL& url);
  virtual blink::WebString doNotTrackValue(blink::WebFrame* frame);
  virtual bool allowWebGL(blink::WebFrame* frame, bool default_value);
  virtual void didLoseWebGLContext(
      blink::WebFrame* frame,
      int arb_robustness_status_code);

  

  virtual void didSerializeDataForFrame(
      const blink::WebURL& frame_url,
      const blink::WebCString& data,
      PageSerializationStatus status) OVERRIDE;

  

  virtual bool Send(IPC::Message* message) OVERRIDE;
  virtual RenderFrame* GetMainRenderFrame() OVERRIDE;
  virtual int GetRoutingID() const OVERRIDE;
  virtual int GetPageId() const OVERRIDE;
  virtual gfx::Size GetSize() const OVERRIDE;
  virtual WebPreferences& GetWebkitPreferences() OVERRIDE;
  virtual void SetWebkitPreferences(const WebPreferences& preferences) OVERRIDE;
  virtual blink::WebView* GetWebView() OVERRIDE;
  virtual blink::WebNode GetFocusedNode() const OVERRIDE;
  virtual blink::WebNode GetContextMenuNode() const OVERRIDE;
  virtual bool IsEditableNode(const blink::WebNode& node) const OVERRIDE;
  virtual void EvaluateScript(const base::string16& frame_xpath,
                              const base::string16& jscript,
                              int id,
                              bool notify_result) OVERRIDE;
  virtual bool ShouldDisplayScrollbars(int width, int height) const OVERRIDE;
  virtual int GetEnabledBindings() const OVERRIDE;
  virtual bool GetContentStateImmediately() const OVERRIDE;
  virtual float GetFilteredTimePerFrame() const OVERRIDE;
  virtual blink::WebPageVisibilityState GetVisibilityState() const OVERRIDE;
  virtual void RunModalAlertDialog(blink::WebFrame* frame,
                                   const blink::WebString& message) OVERRIDE;
  virtual void DidStartLoading() OVERRIDE;
  virtual void DidStopLoading() OVERRIDE;
  virtual void Repaint(const gfx::Size& size) OVERRIDE;
  virtual void SetEditCommandForNextKeyEvent(const std::string& name,
                                             const std::string& value) OVERRIDE;
  virtual void ClearEditCommands() OVERRIDE;
  virtual SSLStatus GetSSLStatusOfFrame(blink::WebFrame* frame) const OVERRIDE;
  virtual const std::string& GetAcceptLanguages() const OVERRIDE;
#if defined(OS_ANDROID)
  virtual void UpdateTopControlsState(TopControlsState constraints,
                                      TopControlsState current,
                                      bool animate) OVERRIDE;
#endif

  

  virtual void DidPlay(blink::WebMediaPlayer* player) OVERRIDE;
  virtual void DidPause(blink::WebMediaPlayer* player) OVERRIDE;
  virtual void PlayerGone(blink::WebMediaPlayer* player) OVERRIDE;

  
  
  

  
  
  typedef std::vector<linked_ptr<ImageResourceFetcher> >
      ImageResourceFetcherList;

 protected:
  
  virtual void Close() OVERRIDE;
  virtual void OnResize(const ViewMsg_Resize_Params& params) OVERRIDE;
  virtual void DidInitiatePaint() OVERRIDE;
  virtual void DidFlushPaint() OVERRIDE;
  virtual PepperPluginInstanceImpl* GetBitmapForOptimizedPluginPaint(
      const gfx::Rect& paint_bounds,
      TransportDIB** dib,
      gfx::Rect* location,
      gfx::Rect* clip,
      float* scale_factor) OVERRIDE;
  virtual gfx::Vector2d GetScrollOffset() OVERRIDE;
  virtual void DidHandleKeyEvent() OVERRIDE;
  virtual bool WillHandleMouseEvent(
      const blink::WebMouseEvent& event) OVERRIDE;
  virtual bool WillHandleKeyEvent(
      const blink::WebKeyboardEvent& event) OVERRIDE;
  virtual bool WillHandleGestureEvent(
      const blink::WebGestureEvent& event) OVERRIDE;
  virtual void DidHandleMouseEvent(const blink::WebMouseEvent& event) OVERRIDE;
  virtual void DidHandleTouchEvent(const blink::WebTouchEvent& event) OVERRIDE;
  virtual bool HasTouchEventHandlersAt(const gfx::Point& point) const OVERRIDE;
  virtual void OnSetFocus(bool enable) OVERRIDE;
  virtual void OnWasHidden() OVERRIDE;
  virtual void OnWasShown(bool needs_repainting) OVERRIDE;
  virtual GURL GetURLForGraphicsContext3D() OVERRIDE;
  virtual bool ForceCompositingModeEnabled() OVERRIDE;
  virtual void OnImeSetComposition(
      const base::string16& text,
      const std::vector<blink::WebCompositionUnderline>& underlines,
      int selection_start,
      int selection_end) OVERRIDE;
  virtual void OnImeConfirmComposition(const base::string16& text,
                                       const gfx::Range& replacement_range,
                                       bool keep_selection) OVERRIDE;
  virtual void SetDeviceScaleFactor(float device_scale_factor) OVERRIDE;
  virtual ui::TextInputType GetTextInputType() OVERRIDE;
  virtual void GetSelectionBounds(gfx::Rect* start, gfx::Rect* end) OVERRIDE;
#if defined(OS_MACOSX) || defined(OS_WIN) || defined(USE_AURA)
  virtual void GetCompositionCharacterBounds(
      std::vector<gfx::Rect>* character_bounds) OVERRIDE;
  virtual void GetCompositionRange(gfx::Range* range) OVERRIDE;
#endif
  virtual bool CanComposeInline() OVERRIDE;
  virtual void DidCommitCompositorFrame() OVERRIDE;
  virtual void InstrumentWillBeginFrame(int frame_id) OVERRIDE;
  virtual void InstrumentDidBeginFrame() OVERRIDE;
  virtual void InstrumentDidCancelFrame() OVERRIDE;
  virtual void InstrumentWillComposite() OVERRIDE;
  virtual bool AllowPartialSwap() const OVERRIDE;

 protected:
  explicit RenderViewImpl(RenderViewImplParams* params);

  void Initialize(RenderViewImplParams* params);
  virtual void SetScreenMetricsEmulationParameters(
      float device_scale_factor,
      const gfx::Point& root_layer_offset,
      float root_layer_scale) OVERRIDE;

  
  virtual ~RenderViewImpl();

 private:
  
  friend class ExternalPopupMenuTest;
  friend class PepperDeviceTest;
  friend class RendererAccessibilityTest;
  friend class RenderViewTest;

  
  
  
  friend class RenderFrameImpl;

  FRIEND_TEST_ALL_PREFIXES(ExternalPopupMenuRemoveTest, RemoveOnChange);
  FRIEND_TEST_ALL_PREFIXES(ExternalPopupMenuTest, NormalCase);
  FRIEND_TEST_ALL_PREFIXES(ExternalPopupMenuTest, ShowPopupThenNavigate);
  FRIEND_TEST_ALL_PREFIXES(RendererAccessibilityTest,
                           AccessibilityMessagesQueueWhileSwappedOut);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, DecideNavigationPolicyForWebUI);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest,
                           DidFailProvisionalLoadWithErrorForError);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest,
                           DidFailProvisionalLoadWithErrorForCancellation);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest,
                           DontIgnoreBackAfterNavEntryLimit);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, ImeComposition);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, InsertCharacters);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, JSBlockSentAfterPageLoad);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, LastCommittedUpdateState);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, OnExtendSelectionAndDelete);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, OnHandleKeyboardEvent);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, OnImeTypeChanged);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, OnNavStateChanged);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, OnSetTextDirection);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, OnUpdateWebPreferences);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, SendSwapOutACK);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, ReloadWhileSwappedOut);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest,
                           SetEditableSelectionAndComposition);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, StaleNavigationsIgnored);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, UpdateTargetURLWithInvalidURL);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest,
                           GetCompositionCharacterBoundsTest);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, OnNavigationHttpPost);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest,
                           DecideNavigationPolicyHandlesAllTopLevel);
#if defined(OS_MACOSX)
  FRIEND_TEST_ALL_PREFIXES(RenderViewTest, MacTestCmdUp);
#endif
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, SetHistoryLengthAndPrune);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, ZoomLimit);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, NavigateFrame);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest,
                           ShouldUpdateSelectionTextFromContextMenuParams);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, BasicRenderFrame);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, TextInputTypeWithPepper);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest,
                           MessageOrderInDidChangeSelection);
  FRIEND_TEST_ALL_PREFIXES(RenderViewImplTest, SendCandidateWindowEvents);
  FRIEND_TEST_ALL_PREFIXES(SuppressErrorPageTest, Suppresses);
  FRIEND_TEST_ALL_PREFIXES(SuppressErrorPageTest, DoesNotSuppress);

  typedef std::map<GURL, double> HostZoomLevels;

  enum ErrorPageType {
    DNS_ERROR,
    HTTP_404,
    CONNECTION_ERROR,
  };

  static blink::WebReferrerPolicy GetReferrerPolicyFromRequest(
      blink::WebFrame* frame,
      const blink::WebURLRequest& request);

  static Referrer GetReferrerFromRequest(
      blink::WebFrame* frame,
      const blink::WebURLRequest& request);

  static webkit_glue::WebURLResponseExtraDataImpl* GetExtraDataFromResponse(
      const blink::WebURLResponse& response);

  void UpdateURL(blink::WebFrame* frame);
  void UpdateTitle(blink::WebFrame* frame, const base::string16& title,
                   blink::WebTextDirection title_direction);
  void UpdateSessionHistory(blink::WebFrame* frame);
  void SendUpdateState(const blink::WebHistoryItem& item);

  
  
  
  
  
  
  
  
  
  
  
  
  
  void UpdateEncoding(blink::WebFrame* frame,
                      const std::string& encoding_name);

  void OpenURL(blink::WebFrame* frame,
               const GURL& url,
               const Referrer& referrer,
               blink::WebNavigationPolicy policy);

  bool RunJavaScriptMessage(JavaScriptMessageType type,
                            const base::string16& message,
                            const base::string16& default_value,
                            const GURL& frame_url,
                            base::string16* result);

  
  bool SendAndRunNestedMessageLoop(IPC::SyncMessage* message);

  
  void UpdateScrollState(blink::WebFrame* frame);

  
  
  
  

  void OnCopy();
  void OnCut();
  void OnDelete();
  void OnExecuteEditCommand(const std::string& name, const std::string& value);
  void OnMoveCaret(const gfx::Point& point);
  void OnPaste();
  void OnPasteAndMatchStyle();
  void OnRedo();
  void OnReplace(const base::string16& text);
  void OnReplaceMisspelling(const base::string16& text);
  void OnScrollFocusedEditableNodeIntoRect(const gfx::Rect& rect);
  void OnSelectAll();
  void OnSelectRange(const gfx::Point& start, const gfx::Point& end);
  void OnSetEditCommandsForNextKeyEvent(const EditCommands& edit_commands);
  void OnUndo();
  void OnUnselect();
  void OnAllowBindings(int enabled_bindings_flags);
  void OnAllowScriptToClose(bool script_can_close);
  void OnCancelDownload(int32 download_id);
  void OnClearFocusedNode();
  void OnClosePage();
  void OnContextMenuClosed(const CustomContextMenuContext& custom_context);
  void OnShowContextMenu(const gfx::Point& location);
  void OnCopyImageAt(int x, int y);
  void OnCSSInsertRequest(const base::string16& frame_xpath,
                          const std::string& css);
  void OnCustomContextMenuAction(const CustomContextMenuContext& custom_context,
      unsigned action);
  void OnSetName(const std::string& name);
  void OnDeterminePageLanguage();
  void OnDisableScrollbarsForSmallWindows(
      const gfx::Size& disable_scrollbars_size_limit);
  void OnDragSourceEndedOrMoved(const gfx::Point& client_point,
                                const gfx::Point& screen_point,
                                bool ended,
                                blink::WebDragOperation drag_operation);
  void OnDragSourceSystemDragEnded();
  void OnDragTargetDrop(const gfx::Point& client_pt,
                        const gfx::Point& screen_pt,
                        int key_modifiers);
  void OnDragTargetDragEnter(const DropData& drop_data,
                             const gfx::Point& client_pt,
                             const gfx::Point& screen_pt,
                             blink::WebDragOperationsMask operations_allowed,
                             int key_modifiers);
  void OnDragTargetDragLeave();
  void OnDragTargetDragOver(const gfx::Point& client_pt,
                            const gfx::Point& screen_pt,
                            blink::WebDragOperationsMask operations_allowed,
                            int key_modifiers);
  void OnEnablePreferredSizeChangedMode();
  void OnEnableAutoResize(const gfx::Size& min_size, const gfx::Size& max_size);
  void OnDisableAutoResize(const gfx::Size& new_size);
  void OnEnumerateDirectoryResponse(int id,
                                    const std::vector<base::FilePath>& paths);
  void OnExtendSelectionAndDelete(int before, int after);
  void OnFileChooserResponse(
      const std::vector<ui::SelectedFileInfo>& files);
  void OnFind(int request_id,
              const base::string16&,
              const blink::WebFindOptions&);
  void OnGetAllSavableResourceLinksForCurrentPage(const GURL& page_url);
  void OnGetSerializedHtmlDataForCurrentPageWithLocalLinks(
      const std::vector<GURL>& links,
      const std::vector<base::FilePath>& local_paths,
      const base::FilePath& local_directory_name);
  void OnMediaPlayerActionAt(const gfx::Point& location,
                             const blink::WebMediaPlayerAction& action);
  void OnOrientationChangeEvent(int orientation);
  void OnPluginActionAt(const gfx::Point& location,
                        const blink::WebPluginAction& action);
  void OnMoveOrResizeStarted();
  void OnNavigate(const ViewMsg_Navigate_Params& params);
  void OnPostMessageEvent(const ViewMsg_PostMessage_Params& params);
  void OnReleaseDisambiguationPopupDIB(TransportDIB::Handle dib_handle);
  void OnReloadFrame();
  void OnResetPageEncodingToDefault();
  void OnScriptEvalRequest(const base::string16& frame_xpath,
                           const base::string16& jscript,
                           int id,
                           bool notify_result);
  void OnSetAccessibilityMode(AccessibilityMode new_mode);
  void OnSetActive(bool active);
  void OnSetAltErrorPageURL(const GURL& gurl);
  void OnSetBackground(const SkBitmap& background);
  void OnSetCompositionFromExistingText(
      int start, int end,
      const std::vector<blink::WebCompositionUnderline>& underlines);
  void OnExitFullscreen();
  void OnSetEditableSelectionOffsets(int start, int end);
  void OnSetHistoryLengthAndPrune(int history_length, int32 minimum_page_id);
  void OnSetInitialFocus(bool reverse);
  void OnSetPageEncoding(const std::string& encoding_name);
  void OnSetRendererPrefs(const RendererPreferences& renderer_prefs);
  void OnSetWebUIProperty(const std::string& name, const std::string& value);
  void OnSetZoomLevel(double zoom_level);
  void OnSetZoomLevelForLoadingURL(const GURL& url, double zoom_level);
  void OnShouldClose();
  void OnStop();
  void OnStopFinding(StopFindAction action);
  void OnSuppressDialogsUntilSwapOut();
  void OnSwapOut();
  void OnThemeChanged();
  void OnUpdateTargetURLAck();
  void OnUpdateTimezone();
  void OnUpdateWebPreferences(const WebPreferences& prefs);
  void OnZoom(PageZoom zoom);
  void OnZoomFactor(PageZoom zoom, int zoom_center_x, int zoom_center_y);
  void OnEnableViewSourceMode();
  void OnDisownOpener();
  void OnWindowSnapshotCompleted(const int snapshot_id,
      const gfx::Size& size, const std::vector<unsigned char>& png);
#if defined(OS_ANDROID)
  void OnActivateNearestFindResult(int request_id, float x, float y);
  void OnFindMatchRects(int current_version);
  void OnSelectPopupMenuItems(bool canceled,
                              const std::vector<int>& selected_indices);
  void OnUndoScrollFocusedEditableNodeIntoRect();
  void OnUpdateTopControlsState(bool enable_hiding,
                                bool enable_showing,
                                bool animate);
  void OnPauseVideo();
  void OnExtractSmartClipData(const gfx::Rect& rect);
#elif defined(OS_MACOSX)
  void OnCopyToFindPboard();
  void OnPluginImeCompositionCompleted(const base::string16& text,
                                       int plugin_id);
  void OnSelectPopupMenuItem(int selected_index);
  void OnSetInLiveResize(bool in_live_resize);
  void OnSetWindowVisibility(bool visible);
  void OnWindowFrameChanged(const gfx::Rect& window_frame,
                            const gfx::Rect& view_frame);
#endif

  
  

  
  void ZoomFactorHelper(PageZoom zoom, int zoom_center_x, int zoom_center_y,
                        float scaling_increment);

  void AltErrorPageFinished(blink::WebFrame* frame,
                            const blink::WebURLRequest& original_request,
                            const blink::WebURLError& original_error,
                            const std::string& html);

  
  void CheckPreferredSize();

  
  
  
  bool InitializeMediaStreamClient();

  
  
  
  void DidDownloadFavicon(ImageResourceFetcher* fetcher,
                          const SkBitmap& image);

  
  
  
  
  
  
  bool DownloadFavicon(int id, const GURL& image_url, int image_size);

  GURL GetAlternateErrorPageURL(const GURL& failed_url,
                                ErrorPageType error_type);

  
  blink::WebFrame* GetChildFrame(const base::string16& frame_xpath) const;

  
  GURL GetLoadingUrl(blink::WebFrame* frame) const;

  
  blink::WebPlugin* GetWebPluginFromPluginDocument();

  
  
  bool IsBackForwardToStaleEntry(const ViewMsg_Navigate_Params& params,
                                 bool is_reload);

  bool MaybeLoadAlternateErrorPage(blink::WebFrame* frame,
                                   const blink::WebURLError& error,
                                   bool replace);

  
  void NavigateToSwappedOutURL(blink::WebFrame* frame);

  
  
  void PopulateDocumentStateFromPending(DocumentState* document_state);

  
  
  NavigationState* CreateNavigationStateFromPending();

  
  
  void ProcessViewLayoutFlags(const CommandLine& command_line);

#if defined(OS_ANDROID)
  
  void LaunchAndroidContentIntent(const GURL& intent_url, size_t request_id);

  blink::WebMediaPlayer* CreateAndroidWebMediaPlayer(
      blink::WebFrame* frame,
      const blink::WebURL& url,
      blink::WebMediaPlayerClient* client);
#endif

  blink::WebMediaPlayer* CreateWebMediaPlayerForMediaStream(
      blink::WebFrame* frame,
      const blink::WebURL& url,
      blink::WebMediaPlayerClient* client);

  
  
  void SendFindReply(int request_id,
                     int match_count,
                     int ordinal,
                     const blink::WebRect& selection_rect,
                     bool final_status_update);

  
  
  static bool ShouldUpdateSelectionTextFromContextMenuParams(
      const base::string16& selection_text,
      size_t selection_text_offset,
      const gfx::Range& selection_range,
      const ContextMenuParams& params);


  
  
  
  static GURL GetOriginalRequestURL(blink::WebDataSource* ds);

  
  void StartNavStateSyncTimerIfNecessary();

  
  
  
  
  void SyncSelectionIfRequired();

#if defined(OS_POSIX) && !defined(OS_MACOSX)
  void UpdateFontRenderingFromRendererPrefs();
#else
  void UpdateFontRenderingFromRendererPrefs() {}
#endif

  
  
  void UpdateTargetURL(const GURL& url, const GURL& fallback_url);

  
  void SendUpdateFaviconURL(const std::vector<FaviconURL>& urls);

  
  
  void DidStopLoadingIcons();

  

  gfx::RectF ClientRectToPhysicalWindowRect(const gfx::RectF& rect) const;

  
  int64 GetLatencyComponentId();

  
  
  
  
  
  ObserverList<RenderViewObserver>& observers() {
    return observers_;
  }

  
  
  void set_is_swapped_out(bool swapped_out) {
    is_swapped_out_ = swapped_out;
  }

  NavigationGesture navigation_gesture() {
    return navigation_gesture_;
  }
  void set_navigation_gesture(NavigationGesture gesture) {
    navigation_gesture_ = gesture;
  }

  
  
  
  

  

  WebPreferences webkit_preferences_;
  RendererPreferences renderer_preferences_;

  HostZoomLevels host_zoom_levels_;

  
  
  
  bool send_content_state_immediately_;

  
  
  int enabled_bindings_;

  
  GURL alternate_error_page_url_;

  
  bool send_preferred_size_changes_;

  
  
  
  
  
  
  
  
  
  gfx::Size disable_scrollbars_size_limit_;

  

  
  bool is_loading_;

  
  NavigationGesture navigation_gesture_;

  
  bool opened_by_user_gesture_;

  
  
  
  bool opener_suppressed_;

  
  
  
  bool suppress_dialogs_until_swap_out_;

  
  
  
  
  scoped_ptr<ViewMsg_Navigate_Params> pending_navigation_params_;

  
  base::OneShotTimer<RenderViewImpl> nav_state_sync_timer_;

  
  
  int32 page_id_;

  
  
  
  
  
  int32 last_page_id_sent_to_browser_;

  
  
  int32 next_page_id_;

  
  int history_list_offset_;

  
  
  
  
  
  int history_list_length_;

  
  
  
  
  std::vector<int32> history_page_ids_;

  

  
  std::string last_encoding_name_;

  

  
  
  
  
  
  
  
  
  enum {
    TARGET_NONE,
    TARGET_INFLIGHT,  
    TARGET_PENDING    
  } target_url_status_;

  
  
  
  
  GURL target_url_;

  
  GURL mouse_over_url_;

  
  GURL focus_url_;

  
  GURL pending_target_url_;

  
  
  
  
  
  base::string16 selection_text_;
  
  size_t selection_text_offset_;
  
  
  gfx::Range selection_range_;

  
  
  
  
  
  
  
  
  
  
  
  IDMap<ContextMenuClient, IDMapExternalPointer> pending_context_menus_;

#if defined(OS_ANDROID)
  
  
  cc::TopControlsState top_controls_constraints_;
#endif

  

  
  
  gfx::Size preferred_size_;

  
  
  base::OneShotTimer<RenderViewImpl> check_preferred_size_timer_;

  
  
  bool cached_is_main_frame_pinned_to_left_;
  bool cached_is_main_frame_pinned_to_right_;

  
  bool cached_has_main_frame_horizontal_scrollbar_;
  bool cached_has_main_frame_vertical_scrollbar_;

  

  scoped_ptr<RenderFrameImpl> main_render_frame_;

  RendererWebCookieJarImpl cookie_jar_;

  
  
  

  
  NotificationProvider* notification_provider_;

  
  GeolocationDispatcher* geolocation_dispatcher_;

  
  InputTagSpeechDispatcher* input_tag_speech_dispatcher_;

  
  
  SpeechRecognitionDispatcher* speech_recognition_dispatcher_;

  
  DeviceOrientationDispatcher* device_orientation_dispatcher_;

  
  MediaStreamDispatcher* media_stream_dispatcher_;

  
  scoped_refptr<BrowserPluginManager> browser_plugin_manager_;

  
  MediaStreamClient* media_stream_client_;
  blink::WebUserMediaClient* web_user_media_client_;

  
  MIDIDispatcher* midi_dispatcher_;

  DevToolsAgent* devtools_agent_;

  
  AccessibilityMode accessibility_mode_;

  
  
  RendererAccessibility* renderer_accessibility_;

  
  MouseLockDispatcher* mouse_lock_dispatcher_;

#if defined(OS_ANDROID)
  

  
  
  
  SkColor body_background_color_;

  
  
  size_t expected_content_intent_id_;

  
  typedef std::vector< linked_ptr<ContentDetector> > ContentDetectorList;
  ContentDetectorList content_detectors_;

  
  
  RendererMediaPlayerManager* media_player_manager_;

  
  scoped_ptr<RendererDateTimePicker> date_time_picker_client_;
#endif

  

  
  
  
  std::set<WebPluginDelegateProxy*> plugin_delegates_;

#if defined(OS_WIN)
  
  int focused_plugin_id_;
#endif

#if defined(ENABLE_PLUGINS)
  
  
  
  

  
  PepperPluginInstanceImpl* focused_pepper_plugin_;

  
  
  
  
  PepperPluginInstanceImpl* pepper_last_mouse_event_target_;
#endif

  

  
  
  
  
  struct PendingFileChooser;
  std::deque< linked_ptr<PendingFileChooser> > file_chooser_completions_;

  
  std::map<int, blink::WebFileChooserCompletion*> enumeration_completions_;
  int enumeration_completion_id_;

  
  scoped_ptr<LoadProgressTracker> load_progress_tracker_;

  
  
  
  int64 session_storage_namespace_id_;

  
  
  EditCommands edit_commands_;

  
  scoped_ptr<ExternalPopupMenu> external_popup_menu_;

  
  blink::WebNode context_menu_node_;

  
  
  ObserverList<RenderViewObserver> observers_;

  
  
  bool handling_select_range_;

  
  scoped_ptr<MouseLockDispatcher::LockTarget> webwidget_mouse_lock_target_;

  
  int next_snapshot_id_;
  typedef std::map<int, WindowSnapshotCallback> PendingSnapshotMap;
  PendingSnapshotMap pending_snapshots_;

  
  
  bool allow_partial_swap_;

  
  
  scoped_ptr<DomAutomationController> dom_automation_controller_;

  
  
  
  DragEventSourceInfo possible_drag_event_info_;

  
  
  scoped_ptr<StatsCollectionObserver> stats_collection_observer_;

  ui::MenuSourceType context_menu_source_type_;
  gfx::Point touch_editing_context_menu_location_;

  
  
  
  
  
  
  
  

  DISALLOW_COPY_AND_ASSIGN(RenderViewImpl);
};

}  

#endif  
