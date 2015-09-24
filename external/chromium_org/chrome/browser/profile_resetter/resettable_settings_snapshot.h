// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILE_RESETTER_RESETTABLE_SETTINGS_SNAPSHOT_H_
#define CHROME_BROWSER_PROFILE_RESETTER_RESETTABLE_SETTINGS_SNAPSHOT_H_

#include "base/basictypes.h"
#include "chrome/browser/prefs/session_startup_pref.h"

namespace base {
class ListValue;
}

class ResettableSettingsSnapshot {
 public:
  
  
  typedef std::vector<std::pair<std::string, std::string> > ExtensionList;
  
  enum Field {
    STARTUP_MODE = 1 << 0,
    HOMEPAGE = 1 << 1,
    DSE_URL = 1 << 2,
    EXTENSIONS = 1 << 3,

    ALL_FIELDS = STARTUP_MODE | HOMEPAGE | DSE_URL | EXTENSIONS,
  };

  explicit ResettableSettingsSnapshot(Profile* profile);
  ~ResettableSettingsSnapshot();

  
  const std::vector<GURL>& startup_urls() const { return startup_.urls; }

  SessionStartupPref::Type startup_type() const { return startup_.type; }

  const std::string& homepage() const { return homepage_; }

  bool homepage_is_ntp() const { return homepage_is_ntp_; }

  const std::string& dse_url() const { return dse_url_; }

  const ExtensionList& enabled_extensions() const {
    return enabled_extensions_;
  }

  
  
  void Subtract(const ResettableSettingsSnapshot& snapshot);

  
  
  
  
  
  int FindDifferentFields(const ResettableSettingsSnapshot& snapshot) const;

 private:
  
  SessionStartupPref startup_;

  std::string homepage_;
  bool homepage_is_ntp_;

  
  std::string dse_url_;

  
  ExtensionList enabled_extensions_;

  DISALLOW_COPY_AND_ASSIGN(ResettableSettingsSnapshot);
};

enum SnapshotCaller {
  PROFILE_RESET_WEBUI = 0,
  PROFILE_RESET_PROMPT,
};

std::string SerializeSettingsReport(const ResettableSettingsSnapshot& snapshot,
                                    int field_mask);

void SendSettingsFeedback(const std::string& report,
                          Profile* profile,
                          SnapshotCaller caller);

base::ListValue* GetReadableFeedback(Profile* profile);

#endif  
