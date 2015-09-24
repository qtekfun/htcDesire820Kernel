// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_HOST_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_HOST_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/perftimer.h"
#include "chrome/browser/extensions/extension_function_dispatcher.h"
#include "chrome/browser/tab_contents/render_view_host_delegate_helper.h"
#include "chrome/browser/ui/app_modal_dialogs/js_modal_dialog.h"
#include "content/browser/renderer_host/render_view_host_delegate.h"
#include "content/common/notification_registrar.h"

#if defined(TOOLKIT_VIEWS)
#include "chrome/browser/ui/views/extensions/extension_view.h"
#elif defined(OS_MACOSX)
#include "chrome/browser/ui/cocoa/extensions/extension_view_mac.h"
#elif defined(TOOLKIT_GTK)
#include "chrome/browser/ui/gtk/extensions/extension_view_gtk.h"
#endif

class Browser;
class Extension;
class FileSelectHelper;
class RenderProcessHost;
class RenderWidgetHostView;
class TabContents;
struct ViewHostMsg_RunFileChooser_Params;
struct WebPreferences;

class ExtensionHost : public RenderViewHostDelegate,
                      public RenderViewHostDelegate::View,
                      public ExtensionFunctionDispatcher::Delegate,
                      public NotificationObserver,
                      public JavaScriptAppModalDialogDelegate {
 public:
  class ProcessCreationQueue;

  
  static void EnableDOMAutomation() { enable_dom_automation_ = true; }

  ExtensionHost(const Extension* extension, SiteInstance* site_instance,
                const GURL& url, ViewType::Type host_type);
  ~ExtensionHost();

#if defined(TOOLKIT_VIEWS)
  void set_view(ExtensionView* view) { view_.reset(view); }
  const ExtensionView* view() const { return view_.get(); }
  ExtensionView* view() { return view_.get(); }
#elif defined(OS_MACOSX)
  const ExtensionViewMac* view() const { return view_.get(); }
  ExtensionViewMac* view() { return view_.get(); }
#elif defined(TOOLKIT_GTK)
  const ExtensionViewGtk* view() const { return view_.get(); }
  ExtensionViewGtk* view() { return view_.get(); }
#endif

  
  
  
  
  void CreateView(Browser* browser);

  const Extension* extension() const { return extension_; }
  const std::string& extension_id() const { return extension_id_; }
  RenderViewHost* render_view_host() const { return render_view_host_; }
  RenderProcessHost* render_process_host() const;
  SiteInstance* site_instance() const;
  bool did_stop_loading() const { return did_stop_loading_; }
  bool document_element_available() const {
    return document_element_available_;
  }

  Profile* profile() const { return profile_; }

  ViewType::Type extension_host_type() const { return extension_host_type_; }

  
  virtual TabContents* associated_tab_contents() const;
  void set_associated_tab_contents(TabContents* associated_tab_contents) {
    associated_tab_contents_ = associated_tab_contents;
  }

  
  bool IsRenderViewLive() const;

  
  
  
  void CreateRenderViewSoon(RenderWidgetHostView* host_view);

  
  void NavigateToURL(const GURL& url);

  
  void InsertInfobarCSS();

  
  
  void DisableScrollbarsForSmallWindows(const gfx::Size& size_limit);

  
  virtual bool OnMessageReceived(const IPC::Message& message);
  virtual const GURL& GetURL() const;
  virtual void RenderViewCreated(RenderViewHost* render_view_host);
  virtual ViewType::Type GetRenderViewType() const;
  virtual int GetBrowserWindowID() const;
  virtual void RenderViewGone(RenderViewHost* render_view_host,
                              base::TerminationStatus status,
                              int error_code);
  virtual void DidNavigate(RenderViewHost* render_view_host,
                           const ViewHostMsg_FrameNavigate_Params& params);
  virtual void DidStopLoading();
  virtual void DocumentAvailableInMainFrame(RenderViewHost* render_view_host);
  virtual void DocumentOnLoadCompletedInMainFrame(
      RenderViewHost* render_view_host,
      int32 page_id);

  
  virtual RenderViewHostDelegate::View* GetViewDelegate();
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
  virtual void ShowContextMenu(const ContextMenuParams& params);
  virtual void ShowPopupMenu(const gfx::Rect& bounds,
                             int item_height,
                             double item_font_size,
                             int selected_item,
                             const std::vector<WebMenuItem>& items,
                             bool right_aligned);
  virtual void StartDragging(const WebDropData& drop_data,
                             WebKit::WebDragOperationsMask allowed_operations,
                             const SkBitmap& image,
                             const gfx::Point& image_offset);
  virtual void UpdateDragCursor(WebKit::WebDragOperation operation);
  virtual void GotFocus();
  virtual void TakeFocus(bool reverse);
  virtual void LostCapture();
  virtual void Activate();
  virtual void Deactivate();
  virtual bool PreHandleKeyboardEvent(const NativeWebKeyboardEvent& event,
                                      bool* is_keyboard_shortcut);
  virtual void HandleKeyboardEvent(const NativeWebKeyboardEvent& event);
  virtual void HandleMouseMove();
  virtual void HandleMouseDown();
  virtual void HandleMouseLeave();
  virtual void HandleMouseUp();
  virtual void HandleMouseActivate();
  virtual void UpdatePreferredSize(const gfx::Size& new_size);
  virtual void UpdateInspectorSetting(const std::string& key,
                                      const std::string& value);
  virtual void ClearInspectorSettings();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void OnMessageBoxClosed(IPC::Message* reply_msg,
                                  bool success,
                                  const std::wstring& prompt);
  virtual void SetSuppressMessageBoxes(bool suppress_message_boxes);
  virtual gfx::NativeWindow GetMessageBoxRootWindow();
  virtual TabContents* AsTabContents();
  virtual ExtensionHost* AsExtensionHost();

 protected:
  
  
  
  
  
  
  
  
  
  virtual RenderWidgetHostView* CreateNewWidgetInternal(
      int route_id,
      WebKit::WebPopupType popup_type);
  virtual void ShowCreatedWidgetInternal(RenderWidgetHostView* widget_host_view,
                                         const gfx::Rect& initial_pos);
 private:
  friend class ProcessCreationQueue;

  
  
  static bool enable_dom_automation_;

  
  void CreateRenderViewNow();

  
  const Browser* GetBrowser() const;

  
  virtual Browser* GetBrowser();
  virtual gfx::NativeView GetNativeViewOfHost();

  
  void OnRunFileChooser(const ViewHostMsg_RunFileChooser_Params& params);

  
  
  
  virtual void UnhandledKeyboardEvent(const NativeWebKeyboardEvent& event) {}

  
  
  bool is_background_page() const { return !view(); }

  
  const Extension* extension_;

  
  const std::string extension_id_;

  
  Profile* profile_;

  
#if defined(TOOLKIT_VIEWS)
  scoped_ptr<ExtensionView> view_;
#elif defined(OS_MACOSX)
  scoped_ptr<ExtensionViewMac> view_;
#elif defined(TOOLKIT_GTK)
  scoped_ptr<ExtensionViewGtk> view_;
#endif

  
  RenderViewHost* render_view_host_;

  
  RenderViewHostDelegateViewHelper delegate_view_helper_;

  
  bool did_stop_loading_;

  
  bool document_element_available_;

  
  GURL url_;

  NotificationRegistrar registrar_;

  scoped_ptr<ExtensionFunctionDispatcher> extension_function_dispatcher_;

  
  
  ViewType::Type extension_host_type_;

  
  TabContents* associated_tab_contents_;

  
  PerfTimer since_created_;

  
  scoped_ptr<FileSelectHelper> file_select_helper_;

  
  base::TimeTicks last_javascript_message_dismissal_;

  
  bool suppress_javascript_messages_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionHost);
};

#endif  
