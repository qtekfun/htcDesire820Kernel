// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_DESTROYER_H_
#define CHROME_BROWSER_PROFILES_PROFILE_DESTROYER_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/timer/timer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;
namespace content {
class RenderProcessHost;
}

class ProfileDestroyer
    : public content::NotificationObserver {
 public:
  static void DestroyProfileWhenAppropriate(Profile* const profile);
  static void DestroyOffTheRecordProfileNow(Profile* const profile);

 private:
  friend class base::RefCounted<ProfileDestroyer>;

  ProfileDestroyer(
      Profile* const profile,
      const std::vector<content::RenderProcessHost*>& hosts);
  virtual ~ProfileDestroyer();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;
  
  void DestroyProfile();

  
  
  static bool GetHostsForProfile(
      Profile* const profile,
      std::vector<content::RenderProcessHost*> *hosts);

  
  static std::vector<ProfileDestroyer*>* pending_destroyers_;

  
  content::NotificationRegistrar registrar_;

  
  base::Timer timer_;

  
  uint32 num_hosts_;

  Profile* profile_;

  base::WeakPtrFactory<ProfileDestroyer> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ProfileDestroyer);
};

#endif  
