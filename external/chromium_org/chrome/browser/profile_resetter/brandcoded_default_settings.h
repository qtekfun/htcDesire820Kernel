// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILE_RESETTER_BRANDCODED_DEFAULT_SETTINGS_H_
#define CHROME_BROWSER_PROFILE_RESETTER_BRANDCODED_DEFAULT_SETTINGS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"

class BrandcodedDefaultSettings {
 public:
  BrandcodedDefaultSettings();
  
  explicit BrandcodedDefaultSettings(const std::string& prefs);
  ~BrandcodedDefaultSettings();

  
  
  
  
  scoped_ptr<ListValue> GetSearchProviderOverrides() const;

  bool GetHomepage(std::string* homepage) const;
  bool GetHomepageIsNewTab(bool* homepage_is_ntp) const;
  bool GetShowHomeButton(bool* show_home_button) const;

  
  bool GetExtensions(std::vector<std::string>* extension_ids) const;

  bool GetRestoreOnStartup(int* restore_on_startup) const;
  scoped_ptr<ListValue> GetUrlsToRestoreOnStartup() const;

 private:
  scoped_ptr<ListValue> ExtractList(const char* pref_name) const;

  scoped_ptr<base::DictionaryValue> master_dictionary_;

  DISALLOW_COPY_AND_ASSIGN(BrandcodedDefaultSettings);
};

#endif  
