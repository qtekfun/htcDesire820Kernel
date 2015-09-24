// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_CONTENTS_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_CONTENTS_H_
#pragma once

#include <string>
#include <vector>

#include "base/scoped_ptr.h"
#include "base/time.h"
#include "chrome/browser/prerender/prerender_final_status.h"
#include "chrome/browser/tab_contents/render_view_host_delegate_helper.h"
#include "chrome/browser/ui/app_modal_dialogs/js_modal_dialog.h"
#include "chrome/common/view_types.h"
#include "content/browser/renderer_host/render_view_host_delegate.h"
#include "content/common/notification_registrar.h"
#include "content/common/window_container_type.h"
#include "webkit/glue/window_open_disposition.h"

class TabContents;
struct FaviconURL;
struct WebPreferences;
struct ViewHostMsg_FrameNavigate_Params;

namespace base {
class ProcessMetrics;
}

namespace gfx {
class Rect;
}

namespace prerender {

class PrerenderManager;

class PrerenderContents : public RenderViewHostDelegate,
                          public RenderViewHostDelegate::View,
                          public NotificationObserver,
                          public JavaScriptAppModalDialogDelegate {
 public:
  
  
  class Factory {
   public:
    Factory() {}
    virtual ~Factory() {}

    virtual PrerenderContents* CreatePrerenderContents(
        PrerenderManager* prerender_manager, Profile* profile, const GURL& url,
        const std::vector<GURL>& alias_urls, const GURL& referrer) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(Factory);
  };

  virtual ~PrerenderContents();

  static Factory* CreateFactory();

  virtual void StartPrerendering();

  
  
  void DestroyWhenUsingTooManyResources();

  RenderViewHost* render_view_host() { return render_view_host_; }
  
  
  
  void set_render_view_host(RenderViewHost* rvh) { render_view_host_ = rvh; }
  ViewHostMsg_FrameNavigate_Params* navigate_params() {
    return navigate_params_.get();
  }
  string16 title() const { return title_; }
  int32 page_id() const { return page_id_; }
  GURL icon_url() const { return icon_url_; }
  bool has_stopped_loading() const { return has_stopped_loading_; }
  bool prerendering_has_started() const { return prerendering_has_started_; }

  
  
  virtual bool GetChildId(int* child_id) const;

  
  
  virtual bool GetRouteId(int* route_id) const;

  
  
  
  void set_final_status(FinalStatus final_status);
  FinalStatus final_status() const;

  base::TimeTicks load_start_time() const { return load_start_time_; }

  
  
  bool MatchesURL(const GURL& url) const;

  
  virtual RenderViewHostDelegate::View* GetViewDelegate();
  virtual const GURL& GetURL() const;
  virtual ViewType::Type GetRenderViewType() const;
  virtual int GetBrowserWindowID() const;
  virtual void DidNavigate(RenderViewHost* render_view_host,
                           const ViewHostMsg_FrameNavigate_Params& params);
  virtual void UpdateTitle(RenderViewHost* render_view_host,
                           int32 page_id,
                           const std::wstring& title);
  virtual WebPreferences GetWebkitPrefs();
  virtual void RunJavaScriptMessage(const std::wstring& message,
                                    const std::wstring& default_prompt,
                                    const GURL& frame_url,
                                    const int flags,
                                    IPC::Message* reply_msg,
                                    bool* did_suppress_message);
  virtual void Close(RenderViewHost* render_view_host);
  virtual void DidStopLoading();
  virtual RendererPreferences GetRendererPrefs(Profile* profile) const;

  
  virtual void CreateNewWindow(
      int route_id,
      const ViewHostMsg_CreateWindow_Params& params);
  virtual void CreateNewWidget(int route_id, WebKit::WebPopupType popup_type);
  virtual void CreateNewFullscreenWidget(int route_id);
  virtual void ShowCreatedWindow(int route_id,
                                 WindowOpenDisposition disposition,
                                 const gfx::Rect& initial_pos,
                                 bool user_gesture);
  virtual void ShowCreatedWidget(int route_id,
                                 const gfx::Rect& initial_pos);
  virtual void ShowCreatedFullscreenWidget(int route_id);
  virtual void ShowContextMenu(const ContextMenuParams& params) {}
  virtual void ShowPopupMenu(const gfx::Rect& bounds,
                             int item_height,
                             double item_font_size,
                             int selected_item,
                             const std::vector<WebMenuItem>& items,
                             bool right_aligned) {}
  virtual void StartDragging(const WebDropData& drop_data,
                             WebKit::WebDragOperationsMask allowed_operations,
                             const SkBitmap& image,
                             const gfx::Point& image_offset) {}
  virtual void UpdateDragCursor(WebKit::WebDragOperation operation) {}
  virtual void GotFocus() {}
  virtual void TakeFocus(bool reverse) {}
  virtual void LostCapture() {}
  virtual void Activate() {}
  virtual void Deactivate() {}
  virtual bool PreHandleKeyboardEvent(const NativeWebKeyboardEvent& event,
                                      bool* is_keyboard_shortcut);
  virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event) {}
  virtual void HandleMouseMove() {}
  virtual void HandleMouseDown() {}
  virtual void HandleMouseLeave() {}
  virtual void HandleMouseUp() {}
  virtual void HandleMouseActivate() {}
  virtual void UpdatePreferredSize(const gfx::Size& new_size) {}

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void OnMessageBoxClosed(IPC::Message* reply_msg,
                                  bool success,
                                  const std::wstring& prompt);
  virtual void SetSuppressMessageBoxes(bool suppress_message_boxes) {}
  virtual gfx::NativeWindow GetMessageBoxRootWindow();
  virtual TabContents* AsTabContents();
  virtual ExtensionHost* AsExtensionHost();

  virtual void UpdateInspectorSetting(const std::string& key,
                                      const std::string& value);
  virtual void ClearInspectorSettings();

  virtual void OnJSOutOfMemory();
  virtual void RendererUnresponsive(RenderViewHost* render_view_host,
                                    bool is_during_unload);

 protected:
  PrerenderContents(PrerenderManager* prerender_manager, Profile* profile,
                    const GURL& url, const std::vector<GURL>& alias_urls,
                    const GURL& referrer);

  
  virtual bool OnMessageReceived(const IPC::Message& message);

  const GURL& prerender_url() const { return prerender_url_; }

 private:
  
  friend class PrerenderContentsFactoryImpl;

  
  void OnDidStartProvisionalLoadForFrame(int64 frame_id,
                                         bool main_frame,
                                         const GURL& url);
  void OnUpdateFaviconURL(int32 page_id, const std::vector<FaviconURL>& urls);
  void OnMaybeCancelPrerenderForHTML5Media();

  
  
  bool AddAliasURL(const GURL& url);

  
  
  void Destroy(FinalStatus reason);

  
  base::ProcessMetrics* MaybeGetProcessMetrics();

  
  PrerenderManager* prerender_manager_;

  
  RenderViewHost* render_view_host_;

  
  RenderViewHostDelegateViewHelper delegate_view_helper_;

  
  GURL prerender_url_;

  
  GURL referrer_;

  
  scoped_ptr<ViewHostMsg_FrameNavigate_Params> navigate_params_;

  
  Profile* profile_;

  
  
  
  
  
  string16 title_;
  int32 page_id_;
  GURL url_;
  GURL icon_url_;
  NotificationRegistrar registrar_;

  
  
  
  std::vector<GURL> alias_urls_;

  bool has_stopped_loading_;

  FinalStatus final_status_;

  bool prerendering_has_started_;

  
  
  
  base::TimeTicks load_start_time_;

  
  
  scoped_ptr<base::ProcessMetrics> process_metrics_;

  
  
  static const int kMaxPrerenderPrivateMB = 100;

  DISALLOW_COPY_AND_ASSIGN(PrerenderContents);
};

}  

#endif  
