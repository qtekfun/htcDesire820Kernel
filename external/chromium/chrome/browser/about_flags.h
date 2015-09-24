// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ABOUT_FLAGS_H_
#define CHROME_BROWSER_ABOUT_FLAGS_H_
#pragma once

#include <map>
#include <string>

#include "base/command_line.h"

class ListValue;
class PrefService;

namespace about_flags {

enum { kOsMac = 1 << 0, kOsWin = 1 << 1, kOsLinux = 1 << 2 , kOsCrOS = 1 << 3 };

struct Experiment {
  enum Type {
    
    SINGLE_VALUE,

    
    
    
    
    
    MULTI_VALUE,
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

  
  const Choice* choices;

  
  
  int num_choices;
};

void ConvertFlagsToSwitches(PrefService* prefs, CommandLine* command_line);

ListValue* GetFlagsExperimentsData(PrefService* prefs);

bool IsRestartNeededToCommitChanges();

void SetExperimentEnabled(
    PrefService* prefs, const std::string& internal_name, bool enable);

void RemoveFlagsSwitches(
    std::map<std::string, CommandLine::StringType>* switch_list);

int GetCurrentPlatform();

void RecordUMAStatistics(const PrefService* prefs);

namespace testing {
void ClearState();

void SetExperiments(const Experiment* e, size_t count);

const Experiment* GetExperiments(size_t* count);

extern const char kMultiSeparator[];

}  

}  

#endif  
