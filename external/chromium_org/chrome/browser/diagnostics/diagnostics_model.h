// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_MODEL_H_
#define CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_MODEL_H_

#include <string>
#include "base/time/time.h"

class CommandLine;

namespace diagnostics {

class DiagnosticsModel {
 public:
  
  enum TestResult {
    TEST_NOT_RUN,
    TEST_RUNNING,
    TEST_OK,
    TEST_FAIL_CONTINUE,
    TEST_FAIL_STOP,
    RECOVERY_RUNNING,
    RECOVERY_OK,
    RECOVERY_FAIL_STOP,
  };

  
  
  static const int kDiagnosticsTestCount;

  
  
  
  
  class Observer {
   public:
    virtual ~Observer() {}
    
    virtual void OnTestFinished(int index, DiagnosticsModel* model) = 0;
    
    virtual void OnAllTestsDone(DiagnosticsModel* model) = 0;
    
    virtual void OnRecoveryFinished(int index, DiagnosticsModel* model) = 0;
    
    virtual void OnAllRecoveryDone(DiagnosticsModel* model) = 0;
  };

  
  class TestInfo {
   public:
    virtual ~TestInfo() {}
    
    
    virtual int GetId() const = 0;
    
    virtual std::string GetName() const = 0;
    
    
    virtual std::string GetTitle() const = 0;
    
    
    virtual TestResult GetResult() const = 0;
    
    
    
    virtual std::string GetAdditionalInfo() const = 0;
    
    
    virtual int GetOutcomeCode() const = 0;
    
    virtual base::Time GetStartTime() const = 0;
    
    virtual base::Time GetEndTime() const = 0;
  };

  virtual ~DiagnosticsModel() {}
  
  virtual int GetTestRunCount() const = 0;
  
  virtual int GetTestAvailableCount() const = 0;
  
  
  
  virtual void RunAll(DiagnosticsModel::Observer* observer) = 0;
  
  virtual void RecoverAll(DiagnosticsModel::Observer* observer) = 0;
  
  
  virtual const TestInfo& GetTest(size_t index) const = 0;
  
  
  
  virtual bool GetTestInfo(int id, const TestInfo** result) const = 0;
};

DiagnosticsModel* MakeDiagnosticsModel(const CommandLine& cmdline);

}  

#endif  
