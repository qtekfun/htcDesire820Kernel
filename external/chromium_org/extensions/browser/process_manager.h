// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_PROCESS_MANAGER_H_
#define EXTENSIONS_BROWSER_PROCESS_MANAGER_H_

#include <map>
#include <set>
#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/common/view_type.h"

class GURL;

namespace content {
class BrowserContext;
class DevToolsAgentHost;
class RenderViewHost;
class SiteInstance;
};

namespace extensions {

class Extension;
class ExtensionHost;

class ProcessManager : public content::NotificationObserver {
 public:
  typedef std::set<extensions::ExtensionHost*> ExtensionHostSet;
  typedef ExtensionHostSet::const_iterator const_iterator;

  static ProcessManager* Create(content::BrowserContext* context);
  virtual ~ProcessManager();

  const ExtensionHostSet& background_hosts() const {
    return background_hosts_;
  }

  typedef std::set<content::RenderViewHost*> ViewSet;
  const ViewSet GetAllViews() const;

  
  
  virtual ExtensionHost* CreateBackgroundHost(const Extension* extension,
                                              const GURL& url);

  
  
  ExtensionHost* GetBackgroundHostForExtension(const std::string& extension_id);

  
  
  
  virtual content::SiteInstance* GetSiteInstanceForURL(const GURL& url);

  
  void UnregisterRenderViewHost(content::RenderViewHost* render_view_host);

  
  
  std::set<content::RenderViewHost*> GetRenderViewHostsForExtension(
      const std::string& extension_id);

  
  
  const Extension* GetExtensionForRenderViewHost(
      content::RenderViewHost* render_view_host);

  
  
  bool IsBackgroundHostClosing(const std::string& extension_id);

  
  
  
  
  int GetLazyKeepaliveCount(const Extension* extension);
  void IncrementLazyKeepaliveCount(const Extension* extension);
  void DecrementLazyKeepaliveCount(const Extension* extension);

  void IncrementLazyKeepaliveCountForView(
      content::RenderViewHost* render_view_host);

  
  
  
  void KeepaliveImpulse(const Extension* extension);

  
  
  void OnShouldSuspendAck(const std::string& extension_id, int sequence_id);

  
  void OnSuspendAck(const std::string& extension_id);

  
  
  void OnNetworkRequestStarted(content::RenderViewHost* render_view_host);
  void OnNetworkRequestDone(content::RenderViewHost* render_view_host);

  
  
  void CancelSuspend(const Extension* extension);

  
  void OnBrowserWindowReady();

  
  
  content::BrowserContext* GetBrowserContext() const;

 protected:
  
  
  ProcessManager(content::BrowserContext* context,
                 content::BrowserContext* original_context);

  
  void CloseBackgroundHosts();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void CreateBackgroundHostsForProfileStartup();

  content::NotificationRegistrar registrar_;

  
  ExtensionHostSet background_hosts_;

  
  
  
  scoped_refptr<content::SiteInstance> site_instance_;

 private:
  friend class ProcessManagerTest;

  
  struct BackgroundPageData;
  typedef std::string ExtensionId;
  typedef std::map<ExtensionId, BackgroundPageData> BackgroundPageDataMap;
  typedef std::map<content::RenderViewHost*,
      extensions::ViewType> ExtensionRenderViews;

  
  void OnBackgroundHostCreated(ExtensionHost* host);

  
  void CloseBackgroundHost(ExtensionHost* host);

  
  
  void DecrementLazyKeepaliveCount(const std::string& extension_id);

  
  void OnKeepaliveImpulseCheck();

  
  
  void OnLazyBackgroundPageIdle(const std::string& extension_id,
                                int sequence_id);
  void OnLazyBackgroundPageActive(const std::string& extension_id);
  void CloseLazyBackgroundPageNow(const std::string& extension_id,
                                  int sequence_id);

  
  
  void RegisterRenderViewHost(content::RenderViewHost* render_view_host);

  
  
  void UnregisterExtension(const std::string& extension_id);

  
  void ClearBackgroundPageData(const std::string& extension_id);

  
  bool DeferLoadingBackgroundHosts() const;

  void OnDevToolsStateChanged(content::DevToolsAgentHost*, bool attached);

  
  
  
  ExtensionRenderViews all_extension_views_;

  BackgroundPageDataMap background_page_data_;

  
  
  base::TimeDelta event_page_idle_time_;

  
  
  base::TimeDelta event_page_suspending_time_;

  
  bool startup_background_hosts_created_;

  base::Callback<void(content::DevToolsAgentHost*, bool)> devtools_callback_;

  base::WeakPtrFactory<ProcessManager> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ProcessManager);
};

}  

#endif  
