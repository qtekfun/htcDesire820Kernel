// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_WARNING_BADGE_SERVICE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_WARNING_BADGE_SERVICE_H_

#include "base/compiler_specific.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/extensions/extension_warning_service.h"
#include "chrome/browser/extensions/extension_warning_set.h"


class Profile;

namespace extensions {

class ExtensionWarningBadgeService : public ExtensionWarningService::Observer,
                                     public base::NonThreadSafe {
 public:
  explicit ExtensionWarningBadgeService(Profile* profile);
  virtual ~ExtensionWarningBadgeService();

  
  
  void SuppressCurrentWarnings();

 protected:
  
  virtual const std::set<ExtensionWarning>& GetCurrentWarnings() const;

 private:
  
  virtual void ExtensionWarningsChanged() OVERRIDE;

  void UpdateBadgeStatus();
  virtual void ShowBadge(bool show);

  Profile* profile_;

  
  ExtensionWarningSet suppressed_warnings_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionWarningBadgeService);
};

}  

#endif  
