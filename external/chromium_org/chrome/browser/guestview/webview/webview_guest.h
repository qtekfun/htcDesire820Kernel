// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GUESTVIEW_WEBVIEW_WEBVIEW_GUEST_H_
#define CHROME_BROWSER_GUESTVIEW_WEBVIEW_WEBVIEW_GUEST_H_

#include "base/observer_list.h"
#include "chrome/browser/extensions/tab_helper.h"
#include "chrome/browser/guestview/guestview.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"

namespace extensions {
class ScriptExecutor;
}  

class WebViewGuest : public GuestView,
                     public content::NotificationObserver,
                     public content::WebContentsObserver {
 public:
  WebViewGuest(content::WebContents* guest_web_contents,
               const std::string& extension_id);

  static WebViewGuest* From(int embedder_process_id, int instance_id);
  static WebViewGuest* FromWebContents(content::WebContents* contents);

  
  virtual void Attach(content::WebContents* embedder_web_contents,
                      const base::DictionaryValue& args) OVERRIDE;
  virtual GuestView::Type GetViewType() const OVERRIDE;
  virtual WebViewGuest* AsWebView() OVERRIDE;
  virtual AdViewGuest* AsAdView() OVERRIDE;

  
  virtual void AddMessageToConsole(int32 level,
                                   const base::string16& message,
                                   int32 line_no,
                                   const base::string16& source_id) OVERRIDE;
  virtual void LoadProgressed(double progress) OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void DidAttach() OVERRIDE;
  virtual void EmbedderDestroyed() OVERRIDE;
  virtual void GuestProcessGone(base::TerminationStatus status) OVERRIDE;
  virtual bool HandleKeyboardEvent(
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual bool IsDragAndDropEnabled() OVERRIDE;
  virtual bool IsOverridingUserAgent() const OVERRIDE;
  virtual void LoadAbort(bool is_top_level,
                         const GURL& url,
                         const std::string& error_type) OVERRIDE;
  virtual void RendererResponsive() OVERRIDE;
  virtual void RendererUnresponsive() OVERRIDE;
  virtual bool RequestPermission(
      BrowserPluginPermissionType permission_type,
      const base::DictionaryValue& request_info,
      const PermissionResponseCallback& callback,
      bool allowed_by_default) OVERRIDE;
  virtual GURL ResolveURL(const std::string& src) OVERRIDE;
  virtual void SizeChanged(const gfx::Size& old_size, const gfx::Size& new_size)
      OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void Go(int relative_index);

  
  void Reload();

  enum PermissionResponseAction {
    DENY,
    ALLOW,
    DEFAULT
  };

  enum SetPermissionResult {
    SET_PERMISSION_INVALID,
    SET_PERMISSION_ALLOWED,
    SET_PERMISSION_DENIED
  };

  
  
  
  SetPermissionResult SetPermission(int request_id,
                                    PermissionResponseAction action,
                                    const std::string& user_input);

  
  
  void SetUserAgentOverride(const std::string& user_agent_override);

  
  void Stop();

  
  void Terminate();

  
  
  
  
  bool ClearData(const base::Time remove_since,
                 uint32 removal_mask,
                 const base::Closure& callback);

  extensions::ScriptExecutor* script_executor() {
    return script_executor_.get();
  }

 private:
  virtual ~WebViewGuest();

  
  struct PermissionResponseInfo {
    PermissionResponseCallback callback;
    BrowserPluginPermissionType permission_type;
    bool allowed_by_default;
    PermissionResponseInfo();
    PermissionResponseInfo(const PermissionResponseCallback& callback,
                           BrowserPluginPermissionType permission_type,
                           bool allowed_by_default);
    ~PermissionResponseInfo();
  };

  static void RecordUserInitiatedUMA(const PermissionResponseInfo& info,
                                     bool allow);
  
  virtual void DidCommitProvisionalLoadForFrame(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& url,
      content::PageTransition transition_type,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidFailProvisionalLoad(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& validated_url,
      int error_code,
      const base::string16& error_description,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidStartProvisionalLoadForFrame(
      int64 frame_id,
      int64 parent_frame_id,
      bool is_main_frame,
      const GURL& validated_url,
      bool is_error_page,
      bool is_iframe_srcdoc,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidStopLoading(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void WebContentsDestroyed(
      content::WebContents* web_contents) OVERRIDE;
  virtual void UserAgentOverrideSet(const std::string& user_agent) OVERRIDE;

  
  void LoadHandlerCalled();

  
  void LoadRedirect(const GURL& old_url,
                    const GURL& new_url,
                    bool is_top_level);

  static bool AllowChromeExtensionURLs();

  void AddWebViewToExtensionRendererState();
  static void RemoveWebViewFromExtensionRendererState(
      content::WebContents* web_contents);

  ObserverList<extensions::TabHelper::ScriptExecutionObserver>
      script_observers_;
  scoped_ptr<extensions::ScriptExecutor> script_executor_;

  content::NotificationRegistrar notification_registrar_;

  
  
  int next_permission_request_id_;

  typedef std::map<int, PermissionResponseInfo> RequestMap;
  RequestMap pending_permission_requests_;

  
  bool is_overriding_user_agent_;

  
  
  bool pending_reload_on_attachment_;

  DISALLOW_COPY_AND_ASSIGN(WebViewGuest);
};

#endif  
