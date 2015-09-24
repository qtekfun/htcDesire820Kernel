// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BROWSER_CONTEXT_KEYED_SERVICE_BROWSER_CONTEXT_DEPENDENCY_MANAGER_H_
#define COMPONENTS_BROWSER_CONTEXT_KEYED_SERVICE_BROWSER_CONTEXT_DEPENDENCY_MANAGER_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_export.h"
#include "components/browser_context_keyed_service/dependency_graph.h"

#ifndef NDEBUG
#include <set>
#endif

class BrowserContextKeyedBaseFactory;

namespace content {
class BrowserContext;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class BROWSER_CONTEXT_KEYED_SERVICE_EXPORT BrowserContextDependencyManager {
 public:
  
  
  void AddComponent(BrowserContextKeyedBaseFactory* component);
  void RemoveComponent(BrowserContextKeyedBaseFactory* component);

  
  void AddEdge(BrowserContextKeyedBaseFactory* depended,
               BrowserContextKeyedBaseFactory* dependee);

  
  
  
  
  void RegisterProfilePrefsForServices(
      const content::BrowserContext* context,
      user_prefs::PrefRegistrySyncable* registry);

  
  
  
  
  
  
  void CreateBrowserContextServices(content::BrowserContext* context);

  
  
  
  
  void CreateBrowserContextServicesForTest(content::BrowserContext* context);

  
  
  void DestroyBrowserContextServices(content::BrowserContext* context);

#ifndef NDEBUG
  
  
  
  void AssertBrowserContextWasntDestroyed(content::BrowserContext* context);
#endif

  static BrowserContextDependencyManager* GetInstance();

 private:
  friend class BrowserContextDependencyManagerUnittests;
  friend struct DefaultSingletonTraits<BrowserContextDependencyManager>;

  
  void DoCreateBrowserContextServices(content::BrowserContext* context,
                                      bool is_testing_context);

  BrowserContextDependencyManager();
  virtual ~BrowserContextDependencyManager();

#ifndef NDEBUG
  void DumpBrowserContextDependencies(content::BrowserContext* context);
#endif

  DependencyGraph dependency_graph_;

#ifndef NDEBUG
  
  
  
  
  std::set<content::BrowserContext*> dead_context_pointers_;
#endif
};

#endif  
