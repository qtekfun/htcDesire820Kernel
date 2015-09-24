// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_PENDING_EXTENSION_INFO_H_
#define CHROME_BROWSER_EXTENSIONS_PENDING_EXTENSION_INFO_H_
#pragma once

#include "chrome/common/extensions/extension.h"

class GURL;

class PendingExtensionInfo {
 public:
  typedef bool (*ShouldAllowInstallPredicate)(const Extension&);

  PendingExtensionInfo(
      const GURL& update_url,
      ShouldAllowInstallPredicate should_allow_install,
      bool is_from_sync,
      bool install_silently,
      bool enable_on_install,
      bool enable_incognito_on_install,
      Extension::Location install_source);

  
  PendingExtensionInfo();

  const GURL& update_url() const { return update_url_; }

  
  
  
  
  
  
  bool ShouldAllowInstall(const Extension& extension) const {
    return should_allow_install_(extension);
  }
  bool is_from_sync() const { return is_from_sync_; }
  bool install_silently() const { return install_silently_; }
  bool enable_on_install() const { return enable_on_install_; }
  bool enable_incognito_on_install() const {
    return enable_incognito_on_install_;
  }
  Extension::Location install_source() const { return install_source_; }

 private:
  GURL update_url_;

  
  
  
  ShouldAllowInstallPredicate should_allow_install_;

  bool is_from_sync_;  
  bool install_silently_;
  bool enable_on_install_;
  bool enable_incognito_on_install_;
  Extension::Location install_source_;

  FRIEND_TEST_ALL_PREFIXES(ExtensionServiceTest, AddPendingExtensionFromSync);
};

#endif  
