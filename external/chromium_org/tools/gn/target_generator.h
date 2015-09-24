// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_TARGET_GENERATOR_H_
#define TOOLS_GN_TARGET_GENERATOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "tools/gn/source_dir.h"
#include "tools/gn/target.h"

class BuildSettings;
class Err;
class FunctionCallNode;
class Location;
class Scope;
class Value;

class TargetGenerator {
 public:
  TargetGenerator(Target* target,
                  Scope* scope,
                  const FunctionCallNode* function_call,
                  Err* err);
  ~TargetGenerator();

  void Run();

  
  
  static void GenerateTarget(Scope* scope,
                             const FunctionCallNode* function_call,
                             const std::vector<Value>& args,
                             const std::string& output_type,
                             Err* err);

 protected:
  
  virtual void DoRun() = 0;

  const BuildSettings* GetBuildSettings() const;

  void FillSources();
  void FillSourcePrereqs();
  void FillConfigs();
  void FillExternal();
  void FillOutputs();

  Target* target_;
  Scope* scope_;
  const FunctionCallNode* function_call_;
  Err* err_;

 private:
  void FillDependentConfigs();  
  void FillData();
  void FillDependencies();  
  void FillGypFile();
  void FillHardDep();

  
  
  void FillGenericConfigs(const char* var_name, LabelConfigVector* dest);
  void FillGenericDeps(const char* var_name, LabelTargetVector* dest);

  void FillForwardDependentConfigs();

  DISALLOW_COPY_AND_ASSIGN(TargetGenerator);
};

#endif  
