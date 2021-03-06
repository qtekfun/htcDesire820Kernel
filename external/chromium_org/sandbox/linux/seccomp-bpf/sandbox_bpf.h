// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SECCOMP_BPF_SANDBOX_BPF_H__
#define SANDBOX_LINUX_SECCOMP_BPF_SANDBOX_BPF_H__

#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <algorithm>
#include <limits>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "sandbox/linux/seccomp-bpf/die.h"
#include "sandbox/linux/seccomp-bpf/errorcode.h"
#include "sandbox/linux/seccomp-bpf/linux_seccomp.h"

namespace sandbox {

struct arch_seccomp_data {
  int nr;
  uint32_t arch;
  uint64_t instruction_pointer;
  uint64_t args[6];
};

struct arch_sigsys {
  void* ip;
  int nr;
  unsigned int arch;
};

class CodeGen;
class SandboxBPFPolicy;
class SandboxUnittestHelper;
struct Instruction;

class SandboxBPF {
 public:
  enum SandboxStatus {
    STATUS_UNKNOWN,      
    STATUS_UNSUPPORTED,  
    STATUS_UNAVAILABLE,  
    STATUS_AVAILABLE,    
    STATUS_ENABLED       
  };

  
  
  
  
  
  typedef ErrorCode (*EvaluateSyscall)(SandboxBPF* sandbox_compiler,
                                       int system_call_number,
                                       void* aux);
  typedef std::vector<std::pair<EvaluateSyscall, void*> > Evaluators;
  
  
  typedef std::vector<struct sock_filter> Program;

  
  
  
  
  
  
  
  
  
  
  SandboxBPF();
  ~SandboxBPF();

  
  
  
  static bool IsValidSyscallNumber(int sysnum);

  
  
  
  
  
  static SandboxStatus SupportsSeccompSandbox(int proc_fd);

  
  
  
  
  
  void set_proc_fd(int proc_fd);

  
  
  
  
  
  
  
  
  
  
  
  
  void SetSandboxPolicyDeprecated(EvaluateSyscall syscallEvaluator, void* aux);

  
  
  void SetSandboxPolicy(SandboxBPFPolicy* policy);

  
  
  
  
  
  
  ErrorCode Trap(Trap::TrapFnc fnc, const void* aux);

  
  
  
  
  
  
  
  
  
  
  ErrorCode UnsafeTrap(Trap::TrapFnc fnc, const void* aux);

  
  
  
  
  
  
  
  
  static intptr_t ForwardSyscall(const struct arch_seccomp_data& args);

  
  
  
  
  
  
  
  
  
  
  ErrorCode Cond(int argno,
                 ErrorCode::ArgType is_32bit,
                 ErrorCode::Operation op,
                 uint64_t value,
                 const ErrorCode& passed,
                 const ErrorCode& failed);

  
  ErrorCode Kill(const char* msg);

  
  
  
  
  
  
  
  
  
  
  
  void StartSandbox();

  
  
  
  
  
  
  
  
  Program* AssembleFilter(bool force_verification);

  
  
  
  ErrorCode Unexpected64bitArgument();

 private:
  friend class CodeGen;
  friend class SandboxUnittestHelper;
  friend class ErrorCode;

  struct Range {
    Range(uint32_t f, uint32_t t, const ErrorCode& e)
        : from(f), to(t), err(e) {}
    uint32_t from, to;
    ErrorCode err;
  };
  typedef std::vector<Range> Ranges;
  typedef std::map<uint32_t, ErrorCode> ErrMap;
  typedef std::set<ErrorCode, struct ErrorCode::LessThan> Conds;

  
  int proc_fd() { return proc_fd_; }

  
  
  
  bool RunFunctionInPolicy(void (*code_in_sandbox)(),
                           EvaluateSyscall syscall_evaluator,
                           void* aux);

  
  
  
  
  bool KernelSupportSeccompBPF();

  
  void PolicySanityChecks(SandboxBPFPolicy* policy);

  
  
  void InstallFilter();

  
  
  
  void VerifyProgram(const Program& program, bool has_unsafe_traps);

  
  
  
  
  void FindRanges(Ranges* ranges);

  
  
  Instruction* AssembleJumpTable(CodeGen* gen,
                                 Ranges::const_iterator start,
                                 Ranges::const_iterator stop);

  
  
  
  
  
  Instruction* RetExpression(CodeGen* gen, const ErrorCode& err);

  
  
  
  
  Instruction* CondExpression(CodeGen* gen, const ErrorCode& cond);

  static SandboxStatus status_;

  bool quiet_;
  int proc_fd_;
  scoped_ptr<const SandboxBPFPolicy> policy_;
  Conds* conds_;
  bool sandbox_has_started_;

  DISALLOW_COPY_AND_ASSIGN(SandboxBPF);
};

}  

#endif  
