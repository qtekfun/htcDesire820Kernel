// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_BROWSER_FIELD_TRIALS_H_
#define CHROME_BROWSER_CHROME_BROWSER_FIELD_TRIALS_H_

#include "base/basictypes.h"
#include "base/command_line.h"

class PrefService;

class ChromeBrowserFieldTrials {
 public:
  explicit ChromeBrowserFieldTrials(const CommandLine& command_line);
  ~ChromeBrowserFieldTrials();

  
  
  void SetupFieldTrials(PrefService* local_state);

 private:
  
  
  void InstantiateDynamicTrials();

  const CommandLine& parsed_command_line_;

  DISALLOW_COPY_AND_ASSIGN(ChromeBrowserFieldTrials);
};

#endif  
