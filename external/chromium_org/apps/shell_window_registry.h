// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APPS_SHELL_WINDOW_REGISTRY_H_
#define APPS_SHELL_WINDOW_REGISTRY_H_

#include <list>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"
#include "ui/gfx/native_widget_types.h"

namespace content {
class BrowserContext;
class DevToolsAgentHost;
class RenderViewHost;
}

namespace apps {

class ShellWindow;

class ShellWindowRegistry : public BrowserContextKeyedService {
 public:
  class Observer {
   public:
    
    virtual void OnShellWindowAdded(apps::ShellWindow* shell_window) = 0;
    
    virtual void OnShellWindowIconChanged(apps::ShellWindow* shell_window) = 0;
    
    virtual void OnShellWindowRemoved(apps::ShellWindow* shell_window) = 0;

   protected:
    virtual ~Observer() {}
  };

  typedef std::list<apps::ShellWindow*> ShellWindowList;
  typedef ShellWindowList::const_iterator const_iterator;
  typedef std::set<std::string> InspectedWindowSet;

  explicit ShellWindowRegistry(content::BrowserContext* context);
  virtual ~ShellWindowRegistry();

  
  
  
  static ShellWindowRegistry* Get(content::BrowserContext* context);

  void AddShellWindow(apps::ShellWindow* shell_window);
  void ShellWindowIconChanged(apps::ShellWindow* shell_window);
  
  void ShellWindowActivated(apps::ShellWindow* shell_window);
  void RemoveShellWindow(apps::ShellWindow* shell_window);

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  ShellWindowList GetShellWindowsForApp(const std::string& app_id) const;
  const ShellWindowList& shell_windows() const { return shell_windows_; }

  
  void CloseAllShellWindowsForApp(const std::string& app_id);

  
  apps::ShellWindow* GetShellWindowForRenderViewHost(
      content::RenderViewHost* render_view_host) const;
  apps::ShellWindow* GetShellWindowForNativeWindow(
      gfx::NativeWindow window) const;
  
  
  
  apps::ShellWindow* GetCurrentShellWindowForApp(
      const std::string& app_id) const;
  
  
  
  
  apps::ShellWindow* GetShellWindowForAppAndKey(
      const std::string& app_id,
      const std::string& window_key) const;

  
  
  
  bool HadDevToolsAttached(content::RenderViewHost* render_view_host) const;

  
  static apps::ShellWindow* GetShellWindowForNativeWindowAnyProfile(
      gfx::NativeWindow window);

  
  
  
  static bool IsShellWindowRegisteredInAnyProfile(int window_type_mask);

  class Factory : public BrowserContextKeyedServiceFactory {
   public:
    static ShellWindowRegistry* GetForBrowserContext(
        content::BrowserContext* context, bool create);

    static Factory* GetInstance();
   private:
    friend struct DefaultSingletonTraits<Factory>;

    Factory();
    virtual ~Factory();

    
    virtual BrowserContextKeyedService* BuildServiceInstanceFor(
        content::BrowserContext* context) const OVERRIDE;
    virtual bool ServiceIsCreatedWithBrowserContext() const OVERRIDE;
    virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;
    virtual content::BrowserContext* GetBrowserContextToUse(
        content::BrowserContext* context) const OVERRIDE;
  };

 protected:
  void OnDevToolsStateChanged(content::DevToolsAgentHost*, bool attached);

 private:
  
  
  void AddShellWindowToList(apps::ShellWindow* shell_window);

  
  
  void BringToFront(apps::ShellWindow* shell_window);

  content::BrowserContext* context_;
  ShellWindowList shell_windows_;
  InspectedWindowSet inspected_windows_;
  ObserverList<Observer> observers_;
  base::Callback<void(content::DevToolsAgentHost*, bool)> devtools_callback_;
};

}  

#endif  
