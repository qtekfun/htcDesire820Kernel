// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ANDROID_CONTENT_VIEW_CORE_IMPL_H_
#define CONTENT_BROWSER_ANDROID_CONTENT_VIEW_CORE_IMPL_H_

#include <vector>

#include "base/android/jni_android.h"
#include "base/android/jni_helper.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/i18n/rtl.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "content/browser/renderer_host/render_widget_host_view_android.h"
#include "content/browser/web_contents/web_contents_impl.h"
#include "content/public/browser/android/content_view_core.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/rect_f.h"
#include "url/gurl.h"

namespace ui {
class ViewAndroid;
class WindowAndroid;
}

namespace content {
class RenderWidgetHostViewAndroid;
struct MenuItem;

class ContentViewCoreImpl : public ContentViewCore,
                            public NotificationObserver,
                            public WebContentsObserver {
 public:
  static ContentViewCoreImpl* FromWebContents(WebContents* web_contents);
  ContentViewCoreImpl(JNIEnv* env,
                      jobject obj,
                      bool hardware_accelerated,
                      WebContents* web_contents,
                      ui::ViewAndroid* view_android,
                      ui::WindowAndroid* window_android);

  
  virtual base::android::ScopedJavaLocalRef<jobject> GetJavaObject() OVERRIDE;
  virtual WebContents* GetWebContents() const OVERRIDE;
  virtual ui::ViewAndroid* GetViewAndroid() const OVERRIDE;
  virtual ui::WindowAndroid* GetWindowAndroid() const OVERRIDE;
  virtual scoped_refptr<cc::Layer> GetLayer() const OVERRIDE;
  virtual void LoadUrl(NavigationController::LoadURLParams& params) OVERRIDE;
  virtual jint GetCurrentRenderProcessId(JNIEnv* env, jobject obj) OVERRIDE;
  virtual void ShowPastePopup(int x, int y) OVERRIDE;
  virtual unsigned int GetScaledContentTexture(
      float scale,
      gfx::Size* out_size) OVERRIDE;
  virtual float GetDpiScale() const OVERRIDE;
  virtual void RequestContentClipping(const gfx::Rect& clipping,
                                      const gfx::Size& content_size) OVERRIDE;
  virtual void PauseVideo() OVERRIDE;
  virtual void PauseOrResumeGeolocation(bool should_pause) OVERRIDE;

  
  
  

  base::android::ScopedJavaLocalRef<jobject> GetWebContentsAndroid(JNIEnv* env,
                                                                   jobject obj);

  void OnJavaContentViewCoreDestroyed(JNIEnv* env, jobject obj);

  
  
  void SelectPopupMenuItems(JNIEnv* env, jobject obj, jintArray indices);

  void LoadUrl(
      JNIEnv* env, jobject obj,
      jstring url,
      jint load_url_type,
      jint transition_type,
      jint ua_override_option,
      jstring extra_headers,
      jbyteArray post_data,
      jstring base_url_for_data_url,
      jstring virtual_url_for_data_url,
      jboolean can_load_local_resources);
  base::android::ScopedJavaLocalRef<jstring> GetURL(JNIEnv* env, jobject) const;
  base::android::ScopedJavaLocalRef<jstring> GetTitle(
      JNIEnv* env, jobject obj) const;
  jboolean IsIncognito(JNIEnv* env, jobject obj);
  void SendOrientationChangeEvent(JNIEnv* env, jobject obj, jint orientation);
  jboolean SendTouchEvent(JNIEnv* env,
                          jobject obj,
                          jlong time_ms,
                          jint type,
                          jobjectArray pts);
  jboolean SendMouseMoveEvent(JNIEnv* env,
                              jobject obj,
                              jlong time_ms,
                              jfloat x,
                              jfloat y);
  jboolean SendMouseWheelEvent(JNIEnv* env,
                               jobject obj,
                               jlong time_ms,
                               jfloat x,
                               jfloat y,
                               jfloat vertical_axis);
  void ScrollBegin(JNIEnv* env, jobject obj, jlong time_ms, jfloat x, jfloat y);
  void ScrollEnd(JNIEnv* env, jobject obj, jlong time_ms);
  void ScrollBy(JNIEnv* env, jobject obj, jlong time_ms,
                jfloat x, jfloat y, jfloat dx, jfloat dy);
  void FlingStart(JNIEnv* env, jobject obj, jlong time_ms,
                  jfloat x, jfloat y, jfloat vx, jfloat vy);
  void FlingCancel(JNIEnv* env, jobject obj, jlong time_ms);
  void SingleTap(JNIEnv* env, jobject obj, jlong time_ms,
                 jfloat x, jfloat y,
                 jboolean disambiguation_popup_tap);
  void SingleTapUnconfirmed(JNIEnv* env, jobject obj, jlong time_ms,
                            jfloat x, jfloat y);
  void ShowPressState(JNIEnv* env, jobject obj, jlong time_ms,
                      jfloat x, jfloat y);
  void TapCancel(JNIEnv* env, jobject obj, jlong time_ms,
                 jfloat x, jfloat y);
  void TapDown(JNIEnv* env, jobject obj, jlong time_ms,
               jfloat x, jfloat y);
  void DoubleTap(JNIEnv* env, jobject obj, jlong time_ms,
                 jfloat x, jfloat y) ;
  void LongPress(JNIEnv* env, jobject obj, jlong time_ms,
                 jfloat x, jfloat y,
                 jboolean disambiguation_popup_tap);
  void LongTap(JNIEnv* env, jobject obj, jlong time_ms,
               jfloat x, jfloat y,
               jboolean disambiguation_popup_tap);
  void PinchBegin(JNIEnv* env, jobject obj, jlong time_ms, jfloat x, jfloat y);
  void PinchEnd(JNIEnv* env, jobject obj, jlong time_ms);
  void PinchBy(JNIEnv* env, jobject obj, jlong time_ms,
               jfloat x, jfloat y, jfloat delta);
  void SelectBetweenCoordinates(JNIEnv* env, jobject obj,
                                jfloat x1, jfloat y1,
                                jfloat x2, jfloat y2);
  void MoveCaret(JNIEnv* env, jobject obj, jfloat x, jfloat y);

  void LoadIfNecessary(JNIEnv* env, jobject obj);
  void RequestRestoreLoad(JNIEnv* env, jobject obj);
  void StopLoading(JNIEnv* env, jobject obj);
  void Reload(JNIEnv* env, jobject obj, jboolean check_for_repost);
  void ReloadIgnoringCache(JNIEnv* env, jobject obj, jboolean check_for_repost);
  void CancelPendingReload(JNIEnv* env, jobject obj);
  void ContinuePendingReload(JNIEnv* env, jobject obj);
  void ClearHistory(JNIEnv* env, jobject obj);
  void EvaluateJavaScript(JNIEnv* env,
                          jobject obj,
                          jstring script,
                          jobject callback,
                          jboolean start_renderer);
  int GetNativeImeAdapter(JNIEnv* env, jobject obj);
  void SetFocus(JNIEnv* env, jobject obj, jboolean focused);
  void ScrollFocusedEditableNodeIntoView(JNIEnv* env, jobject obj);
  void UndoScrollFocusedEditableNodeIntoView(JNIEnv* env, jobject obj);

  jint GetBackgroundColor(JNIEnv* env, jobject obj);
  void SetBackgroundColor(JNIEnv* env, jobject obj, jint color);
  void OnShow(JNIEnv* env, jobject obj);
  void OnHide(JNIEnv* env, jobject obj);
  void ClearSslPreferences(JNIEnv* env, jobject );
  void SetUseDesktopUserAgent(JNIEnv* env,
                              jobject ,
                              jboolean state,
                              jboolean reload_on_state_change);
  bool GetUseDesktopUserAgent(JNIEnv* env, jobject );
  void Show();
  void Hide();
  void AddJavascriptInterface(JNIEnv* env,
                              jobject obj,
                              jobject object,
                              jstring name,
                              jclass safe_annotation_clazz,
                              jobject retained_object_set);
  void RemoveJavascriptInterface(JNIEnv* env, jobject obj, jstring name);
  int GetNavigationHistory(JNIEnv* env, jobject obj, jobject history);
  void GetDirectedNavigationHistory(JNIEnv* env,
                                    jobject obj,
                                    jobject history,
                                    jboolean is_forward,
                                    jint max_entries);
  base::android::ScopedJavaLocalRef<jstring>
      GetOriginalUrlForActiveNavigationEntry(JNIEnv* env, jobject obj);
  void UpdateVSyncParameters(JNIEnv* env, jobject obj, jlong timebase_micros,
                             jlong interval_micros);
  void OnVSync(JNIEnv* env, jobject , jlong frame_time_micros);
  jboolean OnAnimate(JNIEnv* env, jobject , jlong frame_time_micros);
  jboolean PopulateBitmapFromCompositor(JNIEnv* env,
                                        jobject obj,
                                        jobject jbitmap);
  void WasResized(JNIEnv* env, jobject obj);
  jboolean IsRenderWidgetHostViewReady(JNIEnv* env, jobject obj);
  void ExitFullscreen(JNIEnv* env, jobject obj);
  void UpdateTopControlsState(JNIEnv* env,
                              jobject obj,
                              bool enable_hiding,
                              bool enable_showing,
                              bool animate);
  void ShowImeIfNeeded(JNIEnv* env, jobject obj);

  void ShowInterstitialPage(JNIEnv* env,
                            jobject obj,
                            jstring jurl,
                            jint delegate);
  jboolean IsShowingInterstitialPage(JNIEnv* env, jobject obj);

  void AttachExternalVideoSurface(JNIEnv* env,
                                  jobject obj,
                                  jint player_id,
                                  jobject jsurface);
  void DetachExternalVideoSurface(JNIEnv* env, jobject obj, jint player_id);
  void SetAccessibilityEnabled(JNIEnv* env, jobject obj, bool enabled);
  void SendActionAfterDoubleTapUma(JNIEnv* env,
                                   jobject obj,
                                   jint type,
                                   jboolean has_delay,
                                   jint count);
  void SendSingleTapUma(JNIEnv* env, jobject obj, jint type, jint count);

  void ExtractSmartClipData(JNIEnv* env,
                            jobject obj,
                            jint x,
                            jint y,
                            jint width,
                            jint height);
  
  
  

  void OnSmartClipDataExtracted(const string16& result);

  
  
  
  
  void ShowSelectPopupMenu(const std::vector<MenuItem>& items,
                           int selected_item,
                           bool multiple);

  void OnTabCrashed();

  
  void UpdateFrameInfo(const gfx::Vector2dF& scroll_offset,
                       float page_scale_factor,
                       const gfx::Vector2dF& page_scale_factor_limits,
                       const gfx::SizeF& content_size,
                       const gfx::SizeF& viewport_size,
                       const gfx::Vector2dF& controls_offset,
                       const gfx::Vector2dF& content_offset,
                       float overdraw_bottom_height);

  void UpdateImeAdapter(int native_ime_adapter, int text_input_type,
                        const std::string& text,
                        int selection_start, int selection_end,
                        int composition_start, int composition_end,
                        bool show_ime_if_needed, bool require_ack);
  void SetTitle(const base::string16& title);
  void OnBackgroundColorChanged(SkColor color);

  bool HasFocus();
  void ConfirmTouchEvent(InputEventAckState ack_result);
  void UnhandledFlingStartEvent();
  void OnScrollUpdateGestureConsumed();
  void HasTouchEventHandlers(bool need_touch_events);
  void OnSelectionChanged(const std::string& text);
  void OnSelectionBoundsChanged(
      const ViewHostMsg_SelectionBounds_Params& params);

  void StartContentIntent(const GURL& content_url);

  
  
  
  void ShowDisambiguationPopup(
      const gfx::Rect& target_rect, const SkBitmap& zoomed_bitmap);

  
  
  base::android::ScopedJavaLocalRef<jobject> CreateTouchEventSynthesizer();

  
  
  void NotifyExternalSurface(
      int player_id, bool is_request, const gfx::RectF& rect);

  base::android::ScopedJavaLocalRef<jobject> GetContentVideoViewClient();

  
  
  base::android::ScopedJavaLocalRef<jobject> GetContext();

  
  bool ShouldBlockMediaRequest(const GURL& url);

  
  
  

  gfx::Size GetPhysicalBackingSize() const;
  gfx::Size GetViewportSizeDip() const;
  gfx::Size GetViewportSizeOffsetDip() const;
  float GetOverdrawBottomHeightDip() const;

  void AttachLayer(scoped_refptr<cc::Layer> layer);
  void RemoveLayer(scoped_refptr<cc::Layer> layer);
  void AddBeginFrameSubscriber();
  void RemoveBeginFrameSubscriber();
  void SetNeedsAnimate();

 private:
  class ContentViewUserData;

  friend class ContentViewUserData;
  virtual ~ContentViewCoreImpl();

  
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  virtual void RenderViewReady() OVERRIDE;
  virtual void WebContentsDestroyed(WebContents* web_contents) OVERRIDE;

  
  
  

  void InitWebContents();

  RenderWidgetHostViewAndroid* GetRenderWidgetHostViewAndroid();

  float GetTouchPaddingDip();

  blink::WebGestureEvent MakeGestureEvent(
      blink::WebInputEvent::Type type, int64 time_ms, float x, float y) const;

  void SendBeginFrame(base::TimeTicks frame_time);

  gfx::Size GetViewportSizePix() const;
  gfx::Size GetViewportSizeOffsetPix() const;

  void DeleteScaledSnapshotTexture();

  void SendGestureEvent(const blink::WebGestureEvent& event);

  
  void SetFocusInternal(bool focused);

  
  void SendOrientationChangeEventInternal();

  
  JavaObjectWeakGlobalRef java_ref_;

  NotificationRegistrar notification_registrar_;

  
  
  WebContentsImpl* web_contents_;

  
  scoped_refptr<cc::Layer> root_layer_;

  
  float dpi_scale_;

  
  base::TimeDelta vsync_interval_;
  base::TimeDelta expected_browser_composite_time_;

  
  
  ui::ViewAndroid* view_android_;

  
  ui::WindowAndroid* window_android_;

  
  
  int device_orientation_;

  bool geolocation_needs_pause_;

  DISALLOW_COPY_AND_ASSIGN(ContentViewCoreImpl);
};

bool RegisterContentViewCore(JNIEnv* env);

}  

#endif  