// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_BACKGROUND_CONTENTS_H_
#define CHROME_BROWSER_TAB_CONTENTS_BACKGROUND_CONTENTS_H_
#pragma once

#include <string>
#include <vector>

#include "base/scoped_ptr.h"
#include "chrome/browser/tab_contents/render_view_host_delegate_helper.h"
#include "chrome/browser/ui/app_modal_dialogs/js_modal_dialog.h"
#include "chrome/common/view_types.h"
#include "content/browser/renderer_host/render_view_host_delegate.h"
#include "content/common/notification_registrar.h"
#include "content/common/window_container_type.h"
#include "webkit/glue/window_open_disposition.h"

class TabContents;
struct ExtensionHostMsg_DomMessage_Params;
struct WebPreferences;

namespace gfx {
class Rect;
}

class BackgroundContents : public RenderViewHostDelegate,
                           public RenderViewHostDelegate::View,
                           public NotificationObserver,
                           public JavaScriptAppModalDialogDelegate {
 public:
  class Delegate {
   public:
    
    
    
    virtual void AddTabContents(TabContents* new_contents,
                                WindowOpenDisposition disposition,
                                const gfx::Rect& initial_pos,
                                bool user_gesture) = 0;

   protected:
    virtual ~Delegate() {}
  };

  BackgroundContents(SiteInstance* site_instance,
                     int routing_id,
                     Delegate* delegate);
  virtual ~BackgroundContents();

  
  
  RenderViewHost* render_view_host() { return render_view_host_; }

  
  virtual BackgroundContents* GetAsBackgroundContents();
  virtual RenderViewHostDelegate::View* GetViewDelegate();
  virtual const GURL& GetURL() const;
  virtual ViewType::Type GetRenderViewType() const;
  virtual int GetBrowserWindowID() const;
  virtual void DidNavigate(RenderViewHost* render_view_host,
                           const ViewHostMsg_FrameNavigate_Params& params);
  virtual WebPreferences GetWebkitPrefs();
  virtual void ProcessWebUIMessage(
      const ExtensionHostMsg_DomMessage_Params& params);
  virtual void RunJavaScriptMessage(const std::wstring& message,
                                    const std::wstring& default_prompt,
                                    const GURL& frame_url,
                                    const int flags,
                                    IPC::Message* reply_msg,
                                    bool* did_suppress_message);
  virtual void Close(RenderViewHost* render_view_host);
  virtual RendererPreferences GetRendererPrefs(Profile* profile) const;
  virtual void RenderViewGone(RenderViewHost* rvh,
                              base::TerminationStatus status,
                              int error_code);

  
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

  
  
  
  
  static BackgroundContents* GetBackgroundContentsByID(int render_process_id,
                                                       int render_view_id);

 protected:
  
  BackgroundContents();

 private:
  
  Delegate* delegate_;

  
  RenderViewHost* render_view_host_;

  
  RenderViewHostDelegateViewHelper delegate_view_helper_;

  
  GURL url_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundContents);
};

struct BackgroundContentsOpenedDetails {
  
  BackgroundContents* contents;

  
  const string16& frame_name;

  
  const string16& application_id;
};

#endif  
