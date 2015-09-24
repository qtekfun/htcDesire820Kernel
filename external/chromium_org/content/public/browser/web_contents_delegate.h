// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_DELEGATE_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/public/browser/navigation_type.h"
#include "content/public/common/media_stream_request.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/common/window_container_type.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect_f.h"

class GURL;

namespace base {
class FilePath;
class ListValue;
}

namespace content {
class BrowserContext;
class ColorChooser;
class DownloadItem;
class JavaScriptDialogManager;
class PageState;
class RenderViewHost;
class SessionStorageNamespace;
class WebContents;
class WebContentsImpl;
struct ColorSuggestion;
struct ContextMenuParams;
struct DropData;
struct FileChooserParams;
struct NativeWebKeyboardEvent;
struct Referrer;
struct SSLStatus;
}

namespace gfx {
class Point;
class Rect;
class Size;
}

namespace blink {
class WebLayer;
struct WebWindowFeatures;
}

namespace content {

struct OpenURLParams;

class CONTENT_EXPORT WebContentsDelegate {
 public:
  WebContentsDelegate();

  
  
  
  
  
  

  
  
  virtual WebContents* OpenURLFromTab(WebContents* source,
                                      const OpenURLParams& params);

  
  
  
  
  virtual void NavigationStateChanged(const WebContents* source,
                                      unsigned changed_flags) {}

  
  
  virtual void AddNavigationHeaders(const GURL& url, std::string* headers) {}

  
  
  
  
  
  
  virtual void AddNewContents(WebContents* source,
                              WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) {}

  
  virtual void ActivateContents(WebContents* contents) {}

  
  
  virtual void DeactivateContents(WebContents* contents) {}

  
  
  
  virtual void LoadingStateChanged(WebContents* source) {}

  
  
  
  virtual void LoadProgressChanged(WebContents* source,
                                   double progress) {}

  
  
  virtual void CloseContents(WebContents* source) {}

  
  
  virtual void SwappedOut(WebContents* source) {}

  
  
  virtual void MoveContents(WebContents* source, const gfx::Rect& pos) {}

  
  
  virtual bool IsPopupOrPanel(const WebContents* source) const;

  
  virtual void UpdateTargetURL(WebContents* source,
                               int32 page_id,
                               const GURL& url) {}

  
  
  
  virtual void ContentsMouseEvent(WebContents* source,
                                  const gfx::Point& location,
                                  bool motion) {}

  
  virtual void ContentsZoomChange(bool zoom_in) {}

  
  
  virtual bool CanOverscrollContent() const;

  
  
  virtual void OverscrollUpdate(int delta_y) {}

  
  
  virtual bool CanLoadDataURLsInWebUI() const;

  
  
  virtual gfx::Rect GetRootWindowResizerRect() const;

  
  virtual void WillRunBeforeUnloadConfirm() {}

  
  
  virtual bool ShouldSuppressDialogs();

  
  
  
  virtual bool AddMessageToConsole(WebContents* source,
                                   int32 level,
                                   const base::string16& message,
                                   int32 line_no,
                                   const base::string16& source_id);

  
  
  
  
  
  virtual void BeforeUnloadFired(WebContents* tab,
                                 bool proceed,
                                 bool* proceed_to_fire_unload);

  
  
  
  
  virtual bool ShouldFocusLocationBarByDefault(WebContents* source);

  
  
  
  virtual void SetFocusToLocationBar(bool select_all) {}

  
  
  virtual bool ShouldFocusPageAfterCrash();

  
  
  
  virtual void RenderWidgetShowing() {}

  
  
  
  virtual bool TakeFocus(WebContents* source,
                         bool reverse);

  
  virtual void LostCapture() {}

  
  virtual void WebContentsFocused(WebContents* contents) {}

  
  
  virtual void CanDownload(RenderViewHost* render_view_host,
                           int request_id,
                           const std::string& request_method,
                           const base::Callback<void(bool)>& callback);

  
  
  
  virtual int GetExtraRenderViewHeight() const;

  
  virtual bool HandleContextMenu(const content::ContextMenuParams& params);

  
  
  virtual void ViewSourceForTab(WebContents* source, const GURL& page_url);

  
  virtual void ViewSourceForFrame(WebContents* source,
                                  const GURL& url,
                                  const PageState& page_state);

  
  
  
  
  virtual bool PreHandleKeyboardEvent(WebContents* source,
                                      const NativeWebKeyboardEvent& event,
                                      bool* is_keyboard_shortcut);

  
  
  virtual void HandleKeyboardEvent(WebContents* source,
                                   const NativeWebKeyboardEvent& event) {}

  virtual void HandleMouseDown() {}
  virtual void HandleMouseUp() {}

  
  
  virtual void HandlePointerActivate() {}

  virtual void HandleGestureBegin() {}
  virtual void HandleGestureEnd() {}

  
  
  
  virtual bool CanDragEnter(WebContents* source,
                            const DropData& data,
                            blink::WebDragOperationsMask operations_allowed);

  
  virtual void DragEnded() {}

  
  virtual void ShowRepostFormWarningDialog(WebContents* source) {}

  
  
  virtual bool OnGoToEntryOffset(int offset);

  
  
  
  virtual bool ShouldCreateWebContents(
      WebContents* web_contents,
      int route_id,
      WindowContainerType window_container_type,
      const base::string16& frame_name,
      const GURL& target_url,
      const std::string& partition_id,
      SessionStorageNamespace* session_storage_namespace);

  
  
  virtual void WebContentsCreated(WebContents* source_contents,
                                  int64 source_frame_id,
                                  const base::string16& frame_name,
                                  const GURL& target_url,
                                  WebContents* new_contents) {}

  
  virtual void RendererUnresponsive(WebContents* source) {}

  
  virtual void RendererResponsive(WebContents* source) {}

  
  virtual void WorkerCrashed(WebContents* source) {}

  
  virtual void DidNavigateMainFramePostCommit(WebContents* source) {}

  
  
  
  virtual void DidNavigateToPendingEntry(WebContents* source) {}

  
  
  virtual JavaScriptDialogManager* GetJavaScriptDialogManager();

  
  
  
  
  virtual ColorChooser* OpenColorChooser(
      WebContents* web_contents,
      SkColor color,
      const std::vector<ColorSuggestion>& suggestions);

  
  virtual void RunFileChooser(WebContents* web_contents,
                              const FileChooserParams& params) {}

  
  
  
  virtual void EnumerateDirectory(WebContents* web_contents,
                                  int request_id,
                                  const base::FilePath& path) {}

  
  
  
  
  virtual bool EmbedsFullscreenWidget() const;

  
  virtual void ToggleFullscreenModeForTab(WebContents* web_contents,
                                          bool enter_fullscreen) {}
  virtual bool IsFullscreenForTabOrPending(
      const WebContents* web_contents) const;

  
  virtual void JSOutOfMemory(WebContents* web_contents) {}

  
  
  
  virtual void RegisterProtocolHandler(WebContents* web_contents,
                                       const std::string& protocol,
                                       const GURL& url,
                                       const base::string16& title,
                                       bool user_gesture) {}

  
  
  
  virtual void FindReply(WebContents* web_contents,
                         int request_id,
                         int number_of_matches,
                         const gfx::Rect& selection_rect,
                         int active_match_ordinal,
                         bool final_update) {}

#if defined(OS_ANDROID)
  
  
  virtual void FindMatchRectsReply(WebContents* web_contents,
                                   int version,
                                   const std::vector<gfx::RectF>& rects,
                                   const gfx::RectF& active_rect) {}

  
  
  virtual void RequestProtectedMediaIdentifierPermission(
      const WebContents* web_contents,
      const GURL& frame_url,
      const base::Callback<void(bool)>& callback) {}
#endif

  
  virtual void UpdatePreferredSize(WebContents* web_contents,
                                   const gfx::Size& pref_size) {}

  
  virtual void ResizeDueToAutoResize(WebContents* web_contents,
                                     const gfx::Size& new_size) {}

  
  virtual void WebUISend(WebContents* web_contents,
                         const GURL& source_url,
                         const std::string& name,
                         const base::ListValue& args) {}

  
  
  
  virtual void RequestToLockMouse(WebContents* web_contents,
                                  bool user_gesture,
                                  bool last_unlocked_by_target) {}

  
  virtual void LostMouseLock() {}

  
  
  
  
  
  virtual void RequestMediaAccessPermission(
      WebContents* web_contents,
      const MediaStreamRequest& request,
      const MediaResponseCallback& callback);

  
  
  
  virtual bool RequestPpapiBrokerPermission(
      WebContents* web_contents,
      const GURL& url,
      const base::FilePath& plugin_path,
      const base::Callback<void(bool)>& callback);

  
  
  
  
  
  virtual gfx::Size GetSizeForNewRenderView(
      const WebContents* web_contents) const;

  
  
  virtual void ShowValidationMessage(WebContents* web_contents,
                                     const gfx::Rect& anchor_in_root_view,
                                     const string16& main_text,
                                     const string16& sub_text) {}

  
  
  virtual void HideValidationMessage(WebContents* web_contents) {}

  
  
  virtual void MoveValidationMessage(WebContents* web_contents,
                                     const gfx::Rect& anchor_in_root_view) {}

 protected:
  virtual ~WebContentsDelegate();

 private:
  friend class WebContentsImpl;

  
  void Attach(WebContents* source);

  
  void Detach(WebContents* source);

  
  std::set<WebContents*> attached_contents_;
};

}  

#endif  
