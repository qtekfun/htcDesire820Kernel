// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_TEST_H_
#define CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_TEST_H_
#pragma once

#include "base/string16.h"
#include "chrome/browser/diagnostics/diagnostics_model.h"

class FilePath;

class DiagnosticTest : public DiagnosticsModel::TestInfo {
 public:
  
  
  explicit DiagnosticTest(const string16& title);

  virtual ~DiagnosticTest();

  
  
  bool Execute(DiagnosticsModel::Observer* observer, DiagnosticsModel* model,
               size_t index);

  virtual string16 GetTitle();

  virtual DiagnosticsModel::TestResult GetResult();

  virtual string16 GetAdditionalInfo();

  void RecordStopFailure(const string16& additional_info) {
    RecordOutcome(additional_info, DiagnosticsModel::TEST_FAIL_STOP);
  }

  void RecordFailure(const string16& additional_info) {
    RecordOutcome(additional_info, DiagnosticsModel::TEST_FAIL_CONTINUE);
  }

  void RecordSuccess(const string16& additional_info) {
    RecordOutcome(additional_info, DiagnosticsModel::TEST_OK);
  }

  void RecordOutcome(const string16& additional_info,
                     DiagnosticsModel::TestResult result);

  static FilePath GetUserDefaultProfileDir();

 protected:
  
  
  virtual int GetId() = 0;
  
  virtual bool ExecuteImpl(DiagnosticsModel::Observer* observer) = 0;

  string16 title_;
  string16 additional_info_;
  DiagnosticsModel::TestResult result_;
};

#endif  
