// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_USER_PREFS_USER_PREFS_H_
#define COMPONENTS_USER_PREFS_USER_PREFS_H_

#include "base/basictypes.h"
#include "base/supports_user_data.h"
#include "components/user_prefs/user_prefs_export.h"

class PrefService;

namespace content {
class BrowserContext;
}

namespace user_prefs {

class USER_PREFS_EXPORT UserPrefs : public base::SupportsUserData::Data {
 public:
  
  
  static PrefService* Get(content::BrowserContext* context);

  
  
  static void Set(content::BrowserContext* context, PrefService* prefs);

 private:
  explicit UserPrefs(PrefService* prefs);
  virtual ~UserPrefs();

  
  PrefService* prefs_;

  DISALLOW_COPY_AND_ASSIGN(UserPrefs);
};

}  

#endif  
