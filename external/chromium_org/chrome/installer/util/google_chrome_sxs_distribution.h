// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_GOOGLE_CHROME_SXS_DISTRIBUTION_H_
#define CHROME_INSTALLER_UTIL_GOOGLE_CHROME_SXS_DISTRIBUTION_H_

#include "base/strings/string16.h"
#include "chrome/installer/util/browser_distribution.h"
#include "chrome/installer/util/google_chrome_distribution.h"
#include "chrome/installer/util/l10n_string_util.h"
#include "chrome/installer/util/util_constants.h"

class GoogleChromeSxSDistribution : public GoogleChromeDistribution {
 public:
  virtual string16 GetBaseAppName() OVERRIDE;
  virtual string16 GetShortcutName(ShortcutType shortcut_type) OVERRIDE;
  virtual int GetIconIndex(ShortcutType shortcut_type) OVERRIDE;
  virtual string16 GetStartMenuShortcutSubfolder(
      Subfolder subfolder_type) OVERRIDE;
  virtual string16 GetBaseAppId() OVERRIDE;
  virtual string16 GetBrowserProgIdPrefix() OVERRIDE;
  virtual string16 GetBrowserProgIdDesc() OVERRIDE;
  virtual string16 GetInstallSubDir() OVERRIDE;
  virtual string16 GetUninstallRegPath() OVERRIDE;
  virtual DefaultBrowserControlPolicy GetDefaultBrowserControlPolicy() OVERRIDE;
  virtual bool GetChromeChannel(string16* channel) OVERRIDE;
  virtual bool GetCommandExecuteImplClsid(
      string16* handler_class_uuid) OVERRIDE;
  virtual bool AppHostIsSupported() OVERRIDE;
  virtual bool ShouldSetExperimentLabels() OVERRIDE;
  virtual bool HasUserExperiments() OVERRIDE;
  
  static string16 ChannelName();
 private:
  friend class BrowserDistribution;

  
  GoogleChromeSxSDistribution();
};

#endif  
