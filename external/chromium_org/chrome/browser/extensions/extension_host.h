// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_HOST_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_HOST_H_

#include <string>
#include <vector>

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/elapsed_timer.h"
#include "chrome/browser/extensions/extension_function_dispatcher.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "extensions/common/stack_frame.h"
#include "extensions/common/view_type.h"

class PrefsTabHelper;

namespace content {
class BrowserContext;
class RenderProcessHost;
class RenderWidgetHostView;
class SiteInstance;
}

namespace extensions {
class Extension;
class WindowController;

class ExtensionHost : public content::WebContentsDelegate,
                      public content::WebContentsObserver,
                      public ExtensionFunctionDispatcher::Delegate,
                      public content::NotificationObserver {
 public:
  class ProcessCreationQueue;

  ExtensionHost(const Extension* extension,
                content::SiteInstance* site_instance,
                const GURL& url, ViewType host_type);
  virtual ~ExtensionHost();

  const Extension* extension() const { return extension_; }
  const std::string& extension_id() const { return extension_id_; }
  content::WebContents* host_contents() const { return host_contents_.get(); }
  content::RenderViewHost* render_view_host() const;
  content::RenderProcessHost* render_process_host() const;
  bool did_stop_loading() const { return did_stop_loading_; }
  bool document_element_available() const {
    return document_element_available_;
  }

  content::BrowserContext* browser_context() { return browser_context_; }

  ViewType extension_host_type() const { return extension_host_type_; }
  const GURL& GetURL() const;

  
  bool IsRenderViewLive() const;

  
  
  
  void CreateRenderViewSoon();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void RenderViewDeleted(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void RenderViewReady() OVERRIDE;
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;
  virtual void DocumentAvailableInMainFrame() OVERRIDE;
  virtual void DidStopLoading(
      content::RenderViewHost* render_view_host) OVERRIDE;

  
  virtual content::JavaScriptDialogManager*
      GetJavaScriptDialogManager() OVERRIDE;
  virtual void AddNewContents(content::WebContents* source,
                              content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;
  virtual void CloseContents(content::WebContents* contents) OVERRIDE;
  virtual void RequestMediaAccessPermission(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 protected:
  content::NotificationRegistrar* registrar() { return &registrar_; }

  
  
  virtual void OnDidStopLoading();

  
  virtual void OnDocumentAvailable();

  
  virtual void LoadInitialURL();

  
  virtual bool IsBackgroundPage() const;

  
  void Close();

 private:
  friend class ProcessCreationQueue;

  
  void CreateRenderViewNow();

  
  void OnRequest(const ExtensionHostMsg_Request_Params& params);
  void OnEventAck();
  void OnIncrementLazyKeepaliveCount();
  void OnDecrementLazyKeepaliveCount();
  void OnDetailedConsoleMessageAdded(
      const base::string16& message,
      const base::string16& source,
      const StackTrace& stack_trace,
      int32 severity_level);

  
  const Extension* extension_;

  
  const std::string extension_id_;

  
  content::BrowserContext* browser_context_;

  
  scoped_ptr<content::WebContents> host_contents_;

  
  
  
  content::RenderViewHost* render_view_host_;

  
  bool did_stop_loading_;

  
  bool document_element_available_;

  
  GURL initial_url_;

  content::NotificationRegistrar registrar_;

  ExtensionFunctionDispatcher extension_function_dispatcher_;

  
  ViewType extension_host_type_;

  
  base::ElapsedTimer since_created_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionHost);
};

}  

#endif  
