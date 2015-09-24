// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_PROCESS_PROCESS_ITERATOR_H_
#define BASE_PROCESS_PROCESS_ITERATOR_H_

#include <list>
#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/process/process.h"
#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#include <tlhelp32.h>
#elif defined(OS_MACOSX) || defined(OS_BSD)
#include <sys/sysctl.h>
#elif defined(OS_POSIX)
#include <dirent.h>
#endif

namespace base {

#if defined(OS_WIN)
struct ProcessEntry : public PROCESSENTRY32 {
  ProcessId pid() const { return th32ProcessID; }
  ProcessId parent_pid() const { return th32ParentProcessID; }
  const wchar_t* exe_file() const { return szExeFile; }
};

const uint32 kProcessAccessTerminate              = PROCESS_TERMINATE;
const uint32 kProcessAccessCreateThread           = PROCESS_CREATE_THREAD;
const uint32 kProcessAccessSetSessionId           = PROCESS_SET_SESSIONID;
const uint32 kProcessAccessVMOperation            = PROCESS_VM_OPERATION;
const uint32 kProcessAccessVMRead                 = PROCESS_VM_READ;
const uint32 kProcessAccessVMWrite                = PROCESS_VM_WRITE;
const uint32 kProcessAccessDuplicateHandle        = PROCESS_DUP_HANDLE;
const uint32 kProcessAccessCreateProcess          = PROCESS_CREATE_PROCESS;
const uint32 kProcessAccessSetQuota               = PROCESS_SET_QUOTA;
const uint32 kProcessAccessSetInformation         = PROCESS_SET_INFORMATION;
const uint32 kProcessAccessQueryInformation       = PROCESS_QUERY_INFORMATION;
const uint32 kProcessAccessSuspendResume          = PROCESS_SUSPEND_RESUME;
const uint32 kProcessAccessQueryLimitedInfomation =
    PROCESS_QUERY_LIMITED_INFORMATION;
const uint32 kProcessAccessWaitForTermination     = SYNCHRONIZE;
#elif defined(OS_POSIX)
struct BASE_EXPORT ProcessEntry {
  ProcessEntry();
  ~ProcessEntry();

  ProcessId pid() const { return pid_; }
  ProcessId parent_pid() const { return ppid_; }
  ProcessId gid() const { return gid_; }
  const char* exe_file() const { return exe_file_.c_str(); }
  const std::vector<std::string>& cmd_line_args() const {
    return cmd_line_args_;
  }

  ProcessId pid_;
  ProcessId ppid_;
  ProcessId gid_;
  std::string exe_file_;
  std::vector<std::string> cmd_line_args_;
};

const uint32 kProcessAccessTerminate              = 0;
const uint32 kProcessAccessCreateThread           = 0;
const uint32 kProcessAccessSetSessionId           = 0;
const uint32 kProcessAccessVMOperation            = 0;
const uint32 kProcessAccessVMRead                 = 0;
const uint32 kProcessAccessVMWrite                = 0;
const uint32 kProcessAccessDuplicateHandle        = 0;
const uint32 kProcessAccessCreateProcess          = 0;
const uint32 kProcessAccessSetQuota               = 0;
const uint32 kProcessAccessSetInformation         = 0;
const uint32 kProcessAccessQueryInformation       = 0;
const uint32 kProcessAccessSuspendResume          = 0;
const uint32 kProcessAccessQueryLimitedInfomation = 0;
const uint32 kProcessAccessWaitForTermination     = 0;
#endif  

class ProcessFilter {
 public:
  
  
  virtual bool Includes(const ProcessEntry& entry) const = 0;

 protected:
  virtual ~ProcessFilter() {}
};

class BASE_EXPORT ProcessIterator {
 public:
  typedef std::list<ProcessEntry> ProcessEntries;

  explicit ProcessIterator(const ProcessFilter* filter);
  virtual ~ProcessIterator();

  
  
  
  
  
  const ProcessEntry* NextProcessEntry();

  
  ProcessEntries Snapshot();

 protected:
  virtual bool IncludeEntry();
  const ProcessEntry& entry() { return entry_; }

 private:
  
  
  
  bool CheckForNextProcess();

  
  
  void InitProcessEntry(ProcessEntry* entry);

#if defined(OS_WIN)
  HANDLE snapshot_;
  bool started_iteration_;
#elif defined(OS_MACOSX) || defined(OS_BSD)
  std::vector<kinfo_proc> kinfo_procs_;
  size_t index_of_kinfo_proc_;
#elif defined(OS_POSIX)
  DIR* procfs_dir_;
#endif
  ProcessEntry entry_;
  const ProcessFilter* filter_;

  DISALLOW_COPY_AND_ASSIGN(ProcessIterator);
};

class BASE_EXPORT NamedProcessIterator : public ProcessIterator {
 public:
  NamedProcessIterator(const FilePath::StringType& executable_name,
                       const ProcessFilter* filter);
  virtual ~NamedProcessIterator();

 protected:
  virtual bool IncludeEntry() OVERRIDE;

 private:
  FilePath::StringType executable_name_;

  DISALLOW_COPY_AND_ASSIGN(NamedProcessIterator);
};

BASE_EXPORT int GetProcessCount(const FilePath::StringType& executable_name,
                                const ProcessFilter* filter);

}  

#endif  
