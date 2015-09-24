// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_FRAME_REGISTRY_LIST_PREFERENCES_HOLDER_H_
#define CHROME_FRAME_REGISTRY_LIST_PREFERENCES_HOLDER_H_

#include <windows.h>

#include <vector>

#include "base/strings/string16.h"

class RegistryListPreferencesHolder {
 public:
  RegistryListPreferencesHolder();

  
  
  void Init(HKEY hive,
            const wchar_t* registry_path,
            const wchar_t* list_name);

  bool Valid() const { return valid_; }

  
  
  bool ListMatches(const string16& string) const;

  
  void AddStringForTesting(const string16& string);

  
  
  
  void ResetForTesting();

 private:
  std::vector<string16> values_;
  bool valid_;

  DISALLOW_COPY_AND_ASSIGN(RegistryListPreferencesHolder);
};

#endif  
