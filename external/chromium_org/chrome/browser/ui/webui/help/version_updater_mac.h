// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_HELP_VERSION_UPDATER_MAC_H_
#define CHROME_BROWSER_UI_WEBUI_HELP_VERSION_UPDATER_MAC_H_

#import <AppKit/AppKit.h>

#include "base/compiler_specific.h"
#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/webui/help/version_updater.h"

@class KeystoneObserver;

class VersionUpdaterMac : public VersionUpdater {
 public:
  
  virtual void CheckForUpdate(const StatusCallback& status_callback,
                              const PromoteCallback& promote_callback) OVERRIDE;
  virtual void PromoteUpdater() const OVERRIDE;
  virtual void RelaunchBrowser() const OVERRIDE;

  
  
  
  
  void UpdateStatus(NSDictionary* status);

 protected:
  friend class VersionUpdater;

  
  VersionUpdaterMac();
  virtual ~VersionUpdaterMac();

 private:
  
  void UpdateShowPromoteButton();

  
  StatusCallback status_callback_;

  
  PromoteCallback promote_callback_;

  
  bool show_promote_button_;

  
  base::scoped_nsobject<KeystoneObserver> keystone_observer_;

  DISALLOW_COPY_AND_ASSIGN(VersionUpdaterMac);
};

#endif  

