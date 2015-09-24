// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_SETUP_H_
#define TOOLS_GN_SETUP_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "tools/gn/build_settings.h"
#include "tools/gn/builder.h"
#include "tools/gn/loader.h"
#include "tools/gn/scheduler.h"
#include "tools/gn/scope.h"
#include "tools/gn/settings.h"
#include "tools/gn/token.h"
#include "tools/gn/toolchain.h"

class CommandLine;
class InputFile;
class ParseNode;

extern const char kDotfile_Help[];

class CommonSetup {
 public:
  virtual ~CommonSetup();

  
  
  virtual Scheduler* GetScheduler() = 0;

  
  
  void set_check_for_bad_items(bool s) { check_for_bad_items_ = s; }

  BuildSettings& build_settings() { return build_settings_; }
  Builder* builder() { return builder_.get(); }
  LoaderImpl* loader() { return loader_.get(); }

 protected:
  CommonSetup();
  CommonSetup(const CommonSetup& other);

  
  
  void RunPreMessageLoop();
  bool RunPostMessageLoop();

  BuildSettings build_settings_;
  scoped_refptr<LoaderImpl> loader_;
  scoped_refptr<Builder> builder_;

  bool check_for_bad_items_;

 private:
  CommonSetup& operator=(const CommonSetup& other);  
};

class Setup : public CommonSetup {
 public:
  Setup();
  virtual ~Setup();

  
  
  bool DoSetup();

  
  
  
  bool Run();

  Scheduler& scheduler() { return scheduler_; }

  virtual Scheduler* GetScheduler() OVERRIDE;

 private:
  
  bool FillArguments(const CommandLine& cmdline);

  
  bool FillSourceDir(const CommandLine& cmdline);

  
  
  void FillPythonPath();

  
  bool RunConfigFile();

  bool FillOtherConfig(const CommandLine& cmdline);

  Scheduler scheduler_;

  
  
  BuildSettings empty_build_settings_;
  Settings empty_settings_;
  Scope dotfile_scope_;

  
  base::FilePath dotfile_name_;
  scoped_ptr<InputFile> dotfile_input_file_;
  std::vector<Token> dotfile_tokens_;
  scoped_ptr<ParseNode> dotfile_root_;

  
  
  
  scoped_ptr<InputFile> args_input_file_;
  std::vector<Token> args_tokens_;
  scoped_ptr<ParseNode> args_root_;

  DISALLOW_COPY_AND_ASSIGN(Setup);
};

class DependentSetup : public CommonSetup {
 public:
  
  
  
  DependentSetup(Setup* derive_from);
  DependentSetup(DependentSetup* derive_from);
  virtual ~DependentSetup();

  
  
  void RunPreMessageLoop();
  bool RunPostMessageLoop();

  virtual Scheduler* GetScheduler() OVERRIDE;

 private:
  Scheduler* scheduler_;
};

#endif  
