// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_BALLOON_HOST_H_
#define CHROME_BROWSER_NOTIFICATIONS_BALLOON_HOST_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/extension_function_dispatcher.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"

class Balloon;
class Browser;

namespace content {
class SiteInstance;
};

class BalloonHost : public content::WebContentsDelegate,
                    public content::WebContentsObserver,
                    public ExtensionFunctionDispatcher::Delegate {
 public:
  explicit BalloonHost(Balloon* balloon);

  
  void Init();

  
  void Shutdown();

  
  virtual extensions::WindowController* GetExtensionWindowController()
      const OVERRIDE;
  virtual content::WebContents* GetAssociatedWebContents() const OVERRIDE;

  const base::string16& GetSource() const;

  content::WebContents* web_contents() const { return web_contents_.get(); }

  
  void EnableWebUI();

  
  bool IsRenderViewReady() const;

  
  virtual bool CanLoadDataURLsInWebUI() const OVERRIDE;

 protected:
  virtual ~BalloonHost();

  scoped_ptr<content::WebContents> web_contents_;

 private:
  
  virtual void CloseContents(content::WebContents* source) OVERRIDE;
  virtual void HandleMouseDown() OVERRIDE;
  virtual void ResizeDueToAutoResize(content::WebContents* source,
                                     const gfx::Size& pref_size) OVERRIDE;
  virtual void AddNewContents(content::WebContents* source,
                              content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;

  
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void RenderViewReady() OVERRIDE;
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  void OnRequest(const ExtensionHostMsg_Request_Params& params);

  
  
  void NotifyDisconnect();

  
  Balloon* balloon_;

  
  bool initialized_;

  
  
  
  bool should_notify_on_disconnect_;

  
  scoped_refptr<content::SiteInstance> site_instance_;

  
  bool enable_web_ui_;

  ExtensionFunctionDispatcher extension_function_dispatcher_;
};

#endif  
