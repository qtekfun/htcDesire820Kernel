// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_PROCESS_LAUNCH_H_
#define BASE_PROCESS_LAUNCH_H_

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/environment.h"
#include "base/process/process_handle.h"
#include "base/strings/string_piece.h"

#if defined(OS_POSIX)
#include "base/posix/file_descriptor_shuffle.h"
#elif defined(OS_WIN)
#include <windows.h>
#include "base/win/scoped_handle.h"
#endif

class CommandLine;

namespace base {

#if defined(OS_WIN)
typedef std::vector<HANDLE> HandlesToInheritVector;
#endif
typedef std::vector<std::pair<int, int> > FileHandleMappingVector;

struct BASE_EXPORT LaunchOptions {
  LaunchOptions();
  ~LaunchOptions();

  
  bool wait;

#if defined(OS_WIN)
  bool start_hidden;

  
  
  HandlesToInheritVector* handles_to_inherit;

  
  
  
  
  
  
  bool inherit_handles;

  
  
  
  
  
  
  
  UserTokenHandle as_user;

  
  bool empty_desktop_name;

  
  
  
  HANDLE job_handle;

  
  
  
  
  HANDLE stdin_handle;
  HANDLE stdout_handle;
  HANDLE stderr_handle;

  
  
  
  bool force_breakaway_from_job_;
#else
  
  
  EnvironmentMap environ;

  
  
  
  
  const FileHandleMappingVector* fds_to_remap;

  
  
  
  const std::set<int>* maximize_rlimits;

  
  
  
  bool new_process_group;

#if defined(OS_LINUX)
  
  int clone_flags;
#endif  

#if defined(OS_CHROMEOS)
  
  
  int ctrl_terminal_fd;
#endif  

#endif  
};

BASE_EXPORT bool LaunchProcess(const CommandLine& cmdline,
                               const LaunchOptions& options,
                               ProcessHandle* process_handle);

#if defined(OS_WIN)
BASE_EXPORT bool LaunchProcess(const string16& cmdline,
                               const LaunchOptions& options,
                               win::ScopedHandle* process_handle);

#elif defined(OS_POSIX)
BASE_EXPORT bool LaunchProcess(const std::vector<std::string>& argv,
                               const LaunchOptions& options,
                               ProcessHandle* process_handle);

BASE_EXPORT void CloseSuperfluousFds(const InjectiveMultimap& saved_map);
#endif  

#if defined(OS_WIN)
BASE_EXPORT bool SetJobObjectLimitFlags(HANDLE job_object, DWORD limit_flags);

BASE_EXPORT void RouteStdioToConsole();
#endif  

BASE_EXPORT bool GetAppOutput(const CommandLine& cl, std::string* output);

#if defined(OS_WIN)
BASE_EXPORT bool GetAppOutput(const StringPiece16& cl, std::string* output);
#endif

#if defined(OS_POSIX)
BASE_EXPORT bool GetAppOutput(const std::vector<std::string>& argv,
                              std::string* output);

BASE_EXPORT bool GetAppOutputRestricted(const CommandLine& cl,
                                        std::string* output, size_t max_output);

BASE_EXPORT bool GetAppOutputWithExitCode(const CommandLine& cl,
                                          std::string* output, int* exit_code);
#endif  

BASE_EXPORT void RaiseProcessToHighPriority();

#if defined(OS_MACOSX)
void RestoreDefaultExceptionHandler();
#endif  

}  

#endif  
