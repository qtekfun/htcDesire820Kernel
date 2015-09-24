// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ABOUT_FLAGS_H_
#define CHROME_BROWSER_ABOUT_FLAGS_H_

#include <map>
#include <string>

#include "base/command_line.h"
#include "base/strings/string16.h"

class PrefService;

namespace base {
class ListValue;
}

namespace about_flags {

class FlagsStorage;

enum { kOsMac = 1 << 0, kOsWin = 1 << 1, kOsLinux = 1 << 2 , kOsCrOS = 1 << 3,
       kOsAndroid = 1 << 4, kOsCrOSOwnerOnly = 1 << 5 };

struct Experiment {
  enum Type {
    
    SINGLE_VALUE,

    
    
    
    
    
    MULTI_VALUE,

    
    
    
    
    ENABLE_DISABLE_VALUE,
  };

  
  
  struct Choice {
    
    int description_id;

    
    const char* command_line_switch;
    
    const char* command_line_value;
  };

  
  
  
  const char* internal_name;

  
  int visible_name_id;

  
  int visible_description_id;

  
  
  unsigned supported_platforms;  

  
  Type type;

  
  
  
  
  const char* command_line_switch;
  
  const char* command_line_value;

  
  
  const char* disable_command_line_switch;
  const char* disable_command_line_value;

  
  const Choice* choices;

  
  
  int num_choices;

  
  std::string NameForChoice(int index) const;

  
  base::string16 DescriptionForChoice(int index) const;
};

enum SentinelsMode { kNoSentinels, kAddSentinels };

void ConvertFlagsToSwitches(FlagsStorage* flags_storage,
                            CommandLine* command_line,
                            SentinelsMode sentinels);

bool AreSwitchesIdenticalToCurrentCommandLine(
    const CommandLine& new_cmdline, const CommandLine& active_cmdline);

enum FlagAccess { kGeneralAccessFlagsOnly, kOwnerAccessToFlags };

void GetFlagsExperimentsData(FlagsStorage* flags_storage,
                             FlagAccess access,
                             base::ListValue* supported_experiments,
                             base::ListValue* unsupported_experiments);

bool IsRestartNeededToCommitChanges();

void SetExperimentEnabled(FlagsStorage* flags_storage,
                          const std::string& internal_name,
                          bool enable);

void RemoveFlagsSwitches(
    std::map<std::string, CommandLine::StringType>* switch_list);

void ResetAllFlags(FlagsStorage* flags_storage);

int GetCurrentPlatform();

void RecordUMAStatistics(FlagsStorage* flags_storage);

namespace testing {

void ClearState();

void SetExperiments(const Experiment* e, size_t count);

const Experiment* GetExperiments(size_t* count);

extern const char kMultiSeparator[];

}  

}  

#endif  
