// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_SHORTCUTS_BACKEND_FACTORY_H_
#define CHROME_BROWSER_HISTORY_SHORTCUTS_BACKEND_FACTORY_H_

#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/refcounted_browser_context_keyed_service_factory.h"

class Profile;

namespace history {
class ShortcutsBackend;
}  

class ShortcutsBackendFactory
    : public RefcountedBrowserContextKeyedServiceFactory {
 public:
  static scoped_refptr<history::ShortcutsBackend> GetForProfile(
      Profile* profile);

  static scoped_refptr<history::ShortcutsBackend> GetForProfileIfExists(
      Profile* profile);

  static ShortcutsBackendFactory* GetInstance();

  
  static scoped_refptr<RefcountedBrowserContextKeyedService>
      BuildProfileForTesting(content::BrowserContext* profile);

  
  
  static scoped_refptr<RefcountedBrowserContextKeyedService>
      BuildProfileNoDatabaseForTesting(content::BrowserContext* profile);

 private:
  friend struct DefaultSingletonTraits<ShortcutsBackendFactory>;

  ShortcutsBackendFactory();
  virtual ~ShortcutsBackendFactory();

  
  virtual scoped_refptr<RefcountedBrowserContextKeyedService>
      BuildServiceInstanceFor(content::BrowserContext* profile) const OVERRIDE;
  virtual bool ServiceIsNULLWhileTesting() const OVERRIDE;
};

#endif  
