// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_TAB_HELPER_H_
#define CHROME_BROWSER_EXTENSIONS_TAB_HELPER_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/extensions/active_tab_permission_granter.h"
#include "chrome/browser/extensions/extension_function_dispatcher.h"
#include "chrome/common/web_application_info.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "extensions/common/stack_frame.h"
#include "third_party/skia/include/core/SkBitmap.h"

namespace content {
struct LoadCommittedDetails;
}

namespace gfx {
class Image;
}

namespace extensions {
class Extension;
class LocationBarController;
class ScriptBadgeController;
class ScriptBubbleController;
class ScriptExecutor;
class WebstoreInlineInstallerFactory;

class TabHelper : public content::WebContentsObserver,
                  public ExtensionFunctionDispatcher::Delegate,
                  public base::SupportsWeakPtr<TabHelper>,
                  public content::NotificationObserver,
                  public content::WebContentsUserData<TabHelper> {
 public:
  
  
  enum WebAppAction {
    NONE,              
    CREATE_SHORTCUT,   
    CREATE_HOSTED_APP, 
    UPDATE_SHORTCUT    
  };

  
  
  class ScriptExecutionObserver {
   public:
    
    typedef std::map<std::string, std::set<std::string> > ExecutingScriptsMap;

    
    
    explicit ScriptExecutionObserver(TabHelper* tab_helper);
    ScriptExecutionObserver();

    
    
    
    
    
    
    virtual void OnScriptsExecuted(
        const content::WebContents* web_contents,
        const ExecutingScriptsMap& executing_scripts_map,
        int32 on_page_id,
        const GURL& on_url) = 0;

   protected:
    virtual ~ScriptExecutionObserver();

    TabHelper* tab_helper_;
  };

  virtual ~TabHelper();

  void AddScriptExecutionObserver(ScriptExecutionObserver* observer) {
    script_execution_observers_.AddObserver(observer);
  }

  void RemoveScriptExecutionObserver(ScriptExecutionObserver* observer) {
    script_execution_observers_.RemoveObserver(observer);
  }

  void CreateApplicationShortcuts();
  void CreateHostedAppFromWebContents();
  bool CanCreateApplicationShortcuts() const;

  void CreateHostedApp(const WebApplicationInfo& info);

  void set_pending_web_app_action(WebAppAction action) {
    pending_web_app_action_ = action;
  }

  

  
  
  
  
  
  
  
  void SetExtensionApp(const Extension* extension);

  
  
  
  void SetExtensionAppById(const std::string& extension_app_id);

  
  void SetExtensionAppIconById(const std::string& extension_app_id);

  const Extension* extension_app() const { return extension_app_; }
  bool is_app() const { return extension_app_ != NULL; }
  const WebApplicationInfo& web_app_info() const {
    return web_app_info_;
  }

  
  
  
  
  
  SkBitmap* GetExtensionAppIcon();

  content::WebContents* web_contents() const {
    return content::WebContentsObserver::web_contents();
  }

  ScriptExecutor* script_executor() {
    return script_executor_.get();
  }

  LocationBarController* location_bar_controller() {
    return location_bar_controller_.get();
  }

  ActiveTabPermissionGranter* active_tab_permission_granter() {
    return active_tab_permission_granter_.get();
  }

  ScriptBubbleController* script_bubble_controller() {
    return script_bubble_controller_.get();
  }

  
  
  void SetAppIcon(const SkBitmap& app_icon);

  
  
  void SetWebstoreInlineInstallerFactoryForTests(
      WebstoreInlineInstallerFactory* factory);

 private:
  explicit TabHelper(content::WebContents* web_contents);
  friend class content::WebContentsUserData<TabHelper>;

  
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void DidCloneToNewWebContents(
      content::WebContents* old_web_contents,
      content::WebContents* new_web_contents) OVERRIDE;

  
  virtual extensions::WindowController* GetExtensionWindowController()
      const OVERRIDE;
  virtual content::WebContents* GetAssociatedWebContents() const OVERRIDE;

  
  void OnDidGetApplicationInfo(int32 page_id, const WebApplicationInfo& info);
  void OnInlineWebstoreInstall(int install_id,
                               int return_route_id,
                               const std::string& webstore_item_id,
                               const GURL& requestor_url);
  void OnGetAppInstallState(const GURL& requestor_url,
                            int return_route_id,
                            int callback_id);
  void OnRequest(const ExtensionHostMsg_Request_Params& params);
  void OnContentScriptsExecuting(
      const ScriptExecutionObserver::ExecutingScriptsMap& extension_ids,
      int32 page_id,
      const GURL& on_url);
  void OnWatchedPageChange(const std::vector<std::string>& css_selectors);
  void OnDetailedConsoleMessageAdded(const base::string16& message,
                                     const base::string16& source,
                                     const StackTrace& stack_trace,
                                     int32 severity_level);

  

  
  
  void UpdateExtensionAppIcon(const Extension* extension);

  const Extension* GetExtension(const std::string& extension_app_id);

  void OnImageLoaded(const gfx::Image& image);

  
  virtual void OnInlineInstallComplete(int install_id,
                                       int return_route_id,
                                       bool success,
                                       const std::string& error);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  void GetApplicationInfo(int32 page_id);

  
  void SetTabId(content::RenderViewHost* render_view_host);

  

  
  
  ObserverList<ScriptExecutionObserver> script_execution_observers_;

  
  
  const Extension* extension_app_;

  
  
  SkBitmap extension_app_icon_;

  
  ExtensionFunctionDispatcher extension_function_dispatcher_;

  
  WebApplicationInfo web_app_info_;

  
  
  WebAppAction pending_web_app_action_;

  content::NotificationRegistrar registrar_;

  scoped_ptr<ScriptExecutor> script_executor_;

  scoped_ptr<LocationBarController> location_bar_controller_;

  scoped_ptr<ActiveTabPermissionGranter> active_tab_permission_granter_;

  scoped_ptr<ScriptBubbleController> script_bubble_controller_;

  Profile* profile_;

  
  base::WeakPtrFactory<TabHelper> image_loader_ptr_factory_;

  
  scoped_ptr<WebstoreInlineInstallerFactory> webstore_inline_installer_factory_;

  DISALLOW_COPY_AND_ASSIGN(TabHelper);
};

}  

#endif  
