// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DEBUGGER_DEVTOOLS_MANAGER_H_
#define CHROME_BROWSER_DEBUGGER_DEVTOOLS_MANAGER_H_
#pragma once

#include <map>
#include <string>

#include "base/memory/ref_counted.h"
#include "chrome/browser/debugger/devtools_client_host.h"
#include "chrome/browser/debugger/devtools_toggle_action.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "webkit/glue/resource_loader_bridge.h"

namespace IPC {
class Message;
}

class DevToolsNetLogObserver;
class GURL;
class IOThread;
class PrefService;
class RenderViewHost;
class TabContentsWraper;

using webkit_glue::ResourceLoaderBridge;

typedef std::map<std::string, std::string> DevToolsRuntimeProperties;

class DevToolsManager : public DevToolsClientHost::CloseListener,
                        public NotificationObserver,
                        public base::RefCounted<DevToolsManager> {
 public:
  static DevToolsManager* GetInstance();

  static void RegisterUserPrefs(PrefService* prefs);

  DevToolsManager();

  
  
  DevToolsClientHost* GetDevToolsClientHostFor(RenderViewHost* inspected_rvh);

  
  
  void RegisterDevToolsClientHostFor(RenderViewHost* inspected_rvh,
                                     DevToolsClientHost* client_host);
  void UnregisterDevToolsClientHostFor(RenderViewHost* inspected_rvh);

  void ForwardToDevToolsAgent(RenderViewHost* client_rvh,
                              const IPC::Message& message);
  void ForwardToDevToolsAgent(DevToolsClientHost* from,
                              const IPC::Message& message);
  void ForwardToDevToolsClient(RenderViewHost* inspected_rvh,
                               const IPC::Message& message);

  void ActivateWindow(RenderViewHost* client_rvn);
  void CloseWindow(RenderViewHost* client_rvn);
  void RequestDockWindow(RenderViewHost* client_rvn);
  void RequestUndockWindow(RenderViewHost* client_rvn);

  void OpenDevToolsWindow(RenderViewHost* inspected_rvh);
  void ToggleDevToolsWindow(RenderViewHost* inspected_rvh,
                            DevToolsToggleAction action);
  void RuntimePropertyChanged(RenderViewHost* inspected_rvh,
                              const std::string& name,
                              const std::string& value);

  
  
  
  void InspectElement(RenderViewHost* inspected_rvh, int x, int y);

  
  
  void OnNavigatingToPendingEntry(RenderViewHost* inspected_rvh,
                                  RenderViewHost* dest_rvh,
                                  const GURL& gurl);

  
  
  void TabReplaced(TabContentsWrapper* old_tab, TabContentsWrapper* new_tab);

  
  
  int DetachClientHost(RenderViewHost* from_rvh);

  
  void AttachClientHost(int client_host_cookie,
                        RenderViewHost* to_rvh);

  
  void CloseAllClientHosts();

 private:
  friend class base::RefCounted<DevToolsManager>;

  virtual ~DevToolsManager();

  
  
  
  
  virtual void ClientHostClosing(DevToolsClientHost* host);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  RenderViewHost* GetInspectedRenderViewHost(DevToolsClientHost* client_host);

  void SendAttachToAgent(RenderViewHost* inspected_rvh);
  void SendDetachToAgent(RenderViewHost* inspected_rvh);

  void ForceReopenWindow();

  DevToolsClientHost* FindOwnerDevToolsClientHost(RenderViewHost* client_rvh);

  void ToggleDevToolsWindow(RenderViewHost* inspected_rvh,
                            bool force_open,
                            DevToolsToggleAction action);

  void ReopenWindow(RenderViewHost* client_rvh, bool docked);

  void BindClientHost(RenderViewHost* inspected_rvh,
                      DevToolsClientHost* client_host,
                      const DevToolsRuntimeProperties& runtime_properties);

  void UnbindClientHost(RenderViewHost* inspected_rvh,
                        DevToolsClientHost* client_host);

  
  
  
  
  
  
  typedef std::map<RenderViewHost*, DevToolsClientHost*>
      InspectedRvhToClientHostMap;
  InspectedRvhToClientHostMap inspected_rvh_to_client_host_;

  typedef std::map<DevToolsClientHost*, RenderViewHost*>
      ClientHostToInspectedRvhMap;
  ClientHostToInspectedRvhMap client_host_to_inspected_rvh_;

  typedef std::map<RenderViewHost*, DevToolsRuntimeProperties>
      RuntimePropertiesMap;
  RuntimePropertiesMap runtime_properties_map_;

  RenderViewHost* inspected_rvh_for_reopen_;
  bool in_initial_show_;

  typedef std::map<int,
                   std::pair<DevToolsClientHost*, DevToolsRuntimeProperties> >
      OrphanClientHosts;
  OrphanClientHosts orphan_client_hosts_;
  int last_orphan_cookie_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(DevToolsManager);
};

#endif  
