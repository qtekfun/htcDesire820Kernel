// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_MODEL_H_
#define CHROME_BROWSER_DIAGNOSTICS_DIAGNOSTICS_MODEL_H_
#pragma once

#include "base/string16.h"

class CommandLine;

class DiagnosticsModel {
 public:
  
  enum TestResult {
    TEST_NOT_RUN,
    TEST_RUNNING,
    TEST_OK,
    TEST_FAIL_CONTINUE,
    TEST_FAIL_STOP,
  };

  
  
  
  
  class Observer {
   public:
    virtual ~Observer() {}
    
    
    virtual void OnProgress(int id, int percent, DiagnosticsModel* model) = 0;
    
    virtual void OnSkipped(int id, DiagnosticsModel* model) = 0;
    
    virtual void OnFinished(int id, DiagnosticsModel* model) = 0;
    
    virtual void OnDoneAll(DiagnosticsModel* model) = 0;
  };

  
  class TestInfo {
   public:
    virtual ~TestInfo() {}
    
    virtual string16 GetTitle() = 0;
    
    
    virtual TestResult GetResult() = 0;
    
    
    virtual string16 GetAdditionalInfo() = 0;
  };

  virtual ~DiagnosticsModel() {}
  
  virtual int GetTestRunCount() = 0;
  
  virtual int GetTestAvailableCount() =0;
  
  
  virtual void RunAll(DiagnosticsModel::Observer* observer) = 0;
  
  
  virtual TestInfo& GetTest(size_t id) = 0;
};

DiagnosticsModel* MakeDiagnosticsModel(const CommandLine& cmdline);


#endif  
