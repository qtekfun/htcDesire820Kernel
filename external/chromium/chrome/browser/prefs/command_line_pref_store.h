// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_COMMAND_LINE_PREF_STORE_H_
#define CHROME_BROWSER_PREFS_COMMAND_LINE_PREF_STORE_H_
#pragma once

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "chrome/browser/prefs/value_map_pref_store.h"

class DictionaryValue;

class CommandLinePrefStore : public ValueMapPrefStore {
 public:
  explicit CommandLinePrefStore(const CommandLine* command_line);
  virtual ~CommandLinePrefStore();

 protected:
  
  
  bool ValidateProxySwitches();

 private:
  struct StringSwitchToPreferenceMapEntry {
    const char* switch_name;
    const char* preference_path;
  };

  
  
  struct BooleanSwitchToPreferenceMapEntry {
    const char* switch_name;
    const char* preference_path;
    bool set_value;
  };
  static const BooleanSwitchToPreferenceMapEntry boolean_switch_map_[];

  
  
  void ApplySimpleSwitches();

  
  void ApplyProxyMode();

  
  const CommandLine* command_line_;

  static const StringSwitchToPreferenceMapEntry string_switch_map_[];

  DISALLOW_COPY_AND_ASSIGN(CommandLinePrefStore);
};

#endif  
