// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_HELP_VERSION_UPDATER_H_
#define CHROME_BROWSER_UI_WEBUI_HELP_VERSION_UPDATER_H_

#include <string>

#include "base/callback.h"
#include "base/strings/string16.h"

class VersionUpdater {
 public:
  
  enum Status {
    CHECKING,
    UPDATING,
    NEARLY_UPDATED,
    UPDATED,
    FAILED,
    FAILED_OFFLINE,
    FAILED_CONNECTION_TYPE_DISALLOWED,
    DISABLED,
  };

#if defined(OS_MACOSX)
  
  enum PromotionState {
    PROMOTE_HIDDEN,
    PROMOTE_ENABLED,
    PROMOTE_DISABLED
  };
#endif  

  
  
#if defined(OS_CHROMEOS)
  typedef base::Callback<void(const std::string&)> ChannelCallback;
#endif

  
  
  
  typedef base::Callback<void(Status, int, const base::string16&)>
      StatusCallback;

#if defined(OS_MACOSX)
  
  typedef base::Callback<void(PromotionState)> PromoteCallback;
#endif

  virtual ~VersionUpdater() {}

  
  
  static VersionUpdater* Create();

  
  
  
  virtual void CheckForUpdate(const StatusCallback& status_callback
#if defined(OS_MACOSX)
                              , const PromoteCallback& promote_callback
#endif
                              ) = 0;

#if defined(OS_MACOSX)
  
  virtual void PromoteUpdater() const = 0;
#endif

  
  virtual void RelaunchBrowser() const = 0;

#if defined(OS_CHROMEOS)
  virtual void SetChannel(const std::string& channel,
                          bool is_powerwash_allowed) = 0;
  virtual void GetChannel(bool get_current_channel,
                          const ChannelCallback& callback) = 0;
#endif
};

#endif  
