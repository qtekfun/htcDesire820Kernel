// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_SHELL_WINDOW_GEOMETRY_CACHE_H_
#define CHROME_BROWSER_EXTENSIONS_SHELL_WINDOW_GEOMETRY_CACHE_H_

#include <map>
#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "base/values.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/rect.h"

class Profile;

namespace extensions {
class ExtensionPrefs;
}

namespace apps {

class ShellWindowGeometryCache
    : public BrowserContextKeyedService,
      public content::NotificationObserver {
 public:
  class Factory : public BrowserContextKeyedServiceFactory {
   public:
    static ShellWindowGeometryCache* GetForContext(
        content::BrowserContext* context,
        bool create);

    static Factory* GetInstance();
   private:
    friend struct DefaultSingletonTraits<Factory>;

    Factory();
    virtual ~Factory();

    
    virtual BrowserContextKeyedService* BuildServiceInstanceFor(
        content::BrowserContext* context) const OVERRIDE;
    virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;
    virtual content::BrowserContext* GetBrowserContextToUse(
        content::BrowserContext* context) const OVERRIDE;
  };

  class Observer {
   public:
    virtual void OnGeometryCacheChanged(const std::string& extension_id,
                                        const std::string& window_id,
                                        const gfx::Rect& bounds) = 0;

   protected:
    virtual ~Observer() {};
  };

  ShellWindowGeometryCache(Profile* profile,
                           extensions::ExtensionPrefs* prefs);

  virtual ~ShellWindowGeometryCache();

  
  static ShellWindowGeometryCache* Get(content::BrowserContext* context);

  
  void SaveGeometry(const std::string& extension_id,
                    const std::string& window_id,
                    const gfx::Rect& bounds,
                    const gfx::Rect& screen_bounds,
                    ui::WindowShowState state);

  
  
  
  bool GetGeometry(const std::string& extension_id,
                   const std::string& window_id,
                   gfx::Rect* bounds,
                   gfx::Rect* screen_bounds,
                   ui::WindowShowState* state);

  
  virtual void Shutdown() OVERRIDE;

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  static const size_t kMaxCachedWindows = 100;

 protected:
  friend class ShellWindowGeometryCacheTest;

  
  
  
  void SetSyncDelayForTests(int timeout_ms);

 private:
  
  struct WindowData {
    WindowData();
    ~WindowData();
    gfx::Rect bounds;
    gfx::Rect screen_bounds;
    ui::WindowShowState window_state;
    base::Time last_change;
  };

  
  typedef std::map<std::string, WindowData> ExtensionData;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  void LoadGeometryFromStorage(const std::string& extension_id);
  void OnExtensionUnloaded(const std::string& extension_id);
  void SyncToStorage();

  
  extensions::ExtensionPrefs* prefs_;

  
  std::map<std::string, ExtensionData> cache_;

  
  std::set<std::string> unsynced_extensions_;

  
  base::OneShotTimer<ShellWindowGeometryCache> sync_timer_;

  
  base::TimeDelta sync_delay_;

  content::NotificationRegistrar registrar_;
  ObserverList<Observer> observers_;
};

}  

#endif  
