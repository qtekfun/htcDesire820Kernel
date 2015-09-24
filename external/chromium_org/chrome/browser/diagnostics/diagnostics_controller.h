// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_CONTROLLER_H_
#define CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_CONTROLLER_H_

#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"

class CommandLine;

namespace diagnostics {

class DiagnosticsWriter;
class DiagnosticsModel;

class DiagnosticsController {
 public:
  static DiagnosticsController* GetInstance();

  
  
  int Run(const CommandLine& command_line, DiagnosticsWriter* writer);

  
  
  int RunRecovery(const CommandLine& command_line, DiagnosticsWriter* writer);

  
  
  const DiagnosticsModel& GetResults() const;

  
  bool HasResults();

  
  
  void ClearResults();

  
  
  
  void RecordRegularStartup();

 private:
  friend struct DefaultSingletonTraits<DiagnosticsController>;

  DiagnosticsController();
  ~DiagnosticsController();

  scoped_ptr<DiagnosticsModel> model_;
  DiagnosticsWriter* writer_;

  DISALLOW_COPY_AND_ASSIGN(DiagnosticsController);
};

}  

#endif  
