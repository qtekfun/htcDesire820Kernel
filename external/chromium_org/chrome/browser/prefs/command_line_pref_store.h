// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_COMMAND_LINE_PREF_STORE_H_
#define CHROME_BROWSER_PREFS_COMMAND_LINE_PREF_STORE_H_

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/prefs/value_map_pref_store.h"
#include "base/values.h"

class CommandLinePrefStore : public ValueMapPrefStore {
 public:
  explicit CommandLinePrefStore(const CommandLine* command_line);

 protected:
  virtual ~CommandLinePrefStore();

  
  
  bool ValidateProxySwitches();

 private:
  friend class TestCommandLinePrefStore;

  struct StringSwitchToPreferenceMapEntry {
    const char* switch_name;
    const char* preference_path;
  };

  struct IntegerSwitchToPreferenceMapEntry {
    const char* switch_name;
    const char* preference_path;
  };

  
  
  struct BooleanSwitchToPreferenceMapEntry {
    const char* switch_name;
    const char* preference_path;
    bool set_value;
  };

  
  
  void ApplySimpleSwitches();

  
  void ApplyProxyMode();

  
  void ApplySSLSwitches();

  
  void ApplyBackgroundModeSwitches();

  
  const CommandLine* command_line_;

  
  static const BooleanSwitchToPreferenceMapEntry boolean_switch_map_[];
  static const StringSwitchToPreferenceMapEntry string_switch_map_[];
  static const IntegerSwitchToPreferenceMapEntry integer_switch_map_[];

  DISALLOW_COPY_AND_ASSIGN(CommandLinePrefStore);
};

#endif  
