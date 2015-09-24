// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_TEST_H_
#define CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_TEST_H_

#include "base/compiler_specific.h"
#include "chrome/browser/diagnostics/diagnostics_metrics.h"
#include "chrome/browser/diagnostics/diagnostics_model.h"

namespace base {
class FilePath;
}

namespace diagnostics {

class DiagnosticsTest : public DiagnosticsModel::TestInfo {
 public:
  explicit DiagnosticsTest(DiagnosticsTestId id);

  virtual ~DiagnosticsTest();

  
  
  bool Execute(DiagnosticsModel::Observer* observer, DiagnosticsModel* model,
               size_t index);

  
  
  bool Recover(DiagnosticsModel::Observer* observer, DiagnosticsModel* model,
               size_t index);

  void RecordStopFailure(int outcome_code, const std::string& additional_info) {
    RecordOutcome(
        outcome_code, additional_info, DiagnosticsModel::TEST_FAIL_STOP);
  }

  void RecordFailure(int outcome_code, const std::string& additional_info) {
    RecordOutcome(
        outcome_code, additional_info, DiagnosticsModel::TEST_FAIL_CONTINUE);
  }

  void RecordSuccess(const std::string& additional_info) {
    RecordOutcome(0, additional_info, DiagnosticsModel::TEST_OK);
  }

  void RecordOutcome(int outcome_code,
                     const std::string& additional_info,
                     DiagnosticsModel::TestResult result);

  static base::FilePath GetUserDefaultProfileDir();

  
  virtual int GetId() const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual std::string GetTitle() const OVERRIDE;
  virtual DiagnosticsModel::TestResult GetResult() const OVERRIDE;
  virtual std::string GetAdditionalInfo() const OVERRIDE;
  virtual int GetOutcomeCode() const OVERRIDE;
  virtual base::Time GetStartTime() const OVERRIDE;
  virtual base::Time GetEndTime() const OVERRIDE;
 protected:
  
  virtual bool ExecuteImpl(DiagnosticsModel::Observer* observer) = 0;

  
  
  virtual bool RecoveryImpl(DiagnosticsModel::Observer* observer);

  const DiagnosticsTestId id_;
  std::string additional_info_;
  int outcome_code_;
  DiagnosticsModel::TestResult result_;
  base::Time start_time_;
  base::Time end_time_;
};

}  
#endif  
