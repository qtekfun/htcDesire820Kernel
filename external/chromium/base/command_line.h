// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef BASE_COMMAND_LINE_H_
#define BASE_COMMAND_LINE_H_
#pragma once

#include <stddef.h>
#include <map>
#include <string>
#include <vector>

#include "base/base_api.h"
#include "build/build_config.h"

class FilePath;

class BASE_API CommandLine {
 public:
#if defined(OS_WIN)
  
  typedef std::wstring StringType;
#elif defined(OS_POSIX)
  typedef std::string StringType;
#endif

  typedef std::vector<StringType> StringVector;
  
  typedef std::map<std::string, StringType> SwitchMap;

  
  enum NoProgram { NO_PROGRAM };
  explicit CommandLine(NoProgram no_program);

  
  explicit CommandLine(const FilePath& program);

#if defined(OS_POSIX)
  CommandLine(int argc, const char* const* argv);
  explicit CommandLine(const StringVector& argv);
#endif

  ~CommandLine();

  
  
  
  
  static void Init(int argc, const char* const* argv);

  
  
  
  
  static void Reset();

  
  
  
  static CommandLine* ForCurrentProcess();

#if defined(OS_WIN)
  static CommandLine FromString(const std::wstring& command_line);
#endif

#if defined(OS_POSIX)
  
  void InitFromArgv(int argc, const char* const* argv);
  void InitFromArgv(const StringVector& argv);
#endif

  
  
  StringType command_line_string() const;

#if defined(OS_POSIX)
  
  const StringVector& argv() const { return argv_; }
#endif

  
  FilePath GetProgram() const;

  
  
  bool HasSwitch(const std::string& switch_string) const;

  
  
  std::string GetSwitchValueASCII(const std::string& switch_string) const;
  FilePath GetSwitchValuePath(const std::string& switch_string) const;
  StringType GetSwitchValueNative(const std::string& switch_string) const;

  
  
  size_t GetSwitchCount() const;

  
  const SwitchMap& GetSwitches() const { return switches_; }

  
  
  void AppendSwitch(const std::string& switch_string);
  void AppendSwitchPath(const std::string& switch_string, const FilePath& path);
  void AppendSwitchNative(const std::string& switch_string,
                          const StringType& value);
  void AppendSwitchASCII(const std::string& switch_string,
                         const std::string& value);
  void AppendSwitches(const CommandLine& other);

  
  
  void CopySwitchesFrom(const CommandLine& source, const char* const switches[],
                        size_t count);

  
  const StringVector& args() const { return args_; }

  
  
  
  void AppendArg(const std::string& value);
  void AppendArgPath(const FilePath& value);
  void AppendArgNative(const StringType& value);
  void AppendArgs(const CommandLine& other);

  
  
  void AppendArguments(const CommandLine& other,
                       bool include_program);

  
  
  void PrependWrapper(const StringType& wrapper);

#if defined(OS_WIN)
  
  
  void ParseFromString(const std::wstring& command_line);
#endif

 private:
  
  CommandLine();

  
  static CommandLine* current_process_commandline_;

  
  
#if defined(OS_WIN)
  
  StringType command_line_string_;
  
  StringType program_;
#elif defined(OS_POSIX)
  
  StringVector argv_;
#endif

  
  SwitchMap switches_;

  
  StringVector args_;

  
  
  
  
};

#endif  
