// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TABS_PINNED_TAB_SERVICE_FACTORY_H_
#define CHROME_BROWSER_TABS_PINNED_TAB_SERVICE_FACTORY_H_

#include "base/compiler_specific.h"
#include "base/memory/singleton.h"
#include "chrome/browser/profiles/profile_keyed_service_factory.h"

class PinnedTabService;
class Profile;

class PinnedTabServiceFactory : public ProfileKeyedServiceFactory {
 public:
  
  static PinnedTabService* GetForProfile(Profile* profile);

  static PinnedTabServiceFactory* GetInstance();

 private:
  friend struct DefaultSingletonTraits<PinnedTabServiceFactory>;

  PinnedTabServiceFactory();
  virtual ~PinnedTabServiceFactory();

  
  virtual ProfileKeyedService* BuildServiceInstanceFor(
      Profile* profile) const OVERRIDE;
};

#endif  
