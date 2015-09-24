// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_WRITER_H_
#define CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_WRITER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/diagnostics/diagnostics_model.h"

namespace diagnostics {

class SimpleConsole;

class DiagnosticsWriter : public DiagnosticsModel::Observer {
 public:
  
  enum FormatType {
    MACHINE,
    LOG,
    HUMAN
  };

  explicit DiagnosticsWriter(FormatType format);
  virtual ~DiagnosticsWriter();

  
  int failures() { return failures_; }

  
  FormatType format() const { return format_; }

  
  
  bool WriteInfoLine(const std::string& info_text);

  
  virtual void OnTestFinished(int index, DiagnosticsModel* model) OVERRIDE;
  virtual void OnAllTestsDone(DiagnosticsModel* model) OVERRIDE;
  virtual void OnRecoveryFinished(int index, DiagnosticsModel* model) OVERRIDE;
  virtual void OnAllRecoveryDone(DiagnosticsModel* model) OVERRIDE;

 private:
  
  
  
  
  
  bool WriteResult(bool success,
                   const std::string& id,
                   const std::string& name,
                   int outcome_code,
                   const std::string& extra);

  scoped_ptr<SimpleConsole> console_;

  
  int failures_;
  FormatType format_;

  DISALLOW_COPY_AND_ASSIGN(DiagnosticsWriter);
};

}  

#endif  
