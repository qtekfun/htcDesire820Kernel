// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SECCOMP_BPF_HELPERS_SYSCALL_SETS_H_
#define SANDBOX_LINUX_SECCOMP_BPF_HELPERS_SYSCALL_SETS_H_

#include "base/basictypes.h"
#include "build/build_config.h"


namespace sandbox {

class SyscallSets {
 public:
  static bool IsKill(int sysno);
  static bool IsAllowedGettime(int sysno);
  static bool IsCurrentDirectory(int sysno);
  static bool IsUmask(int sysno);
  
  
  
  static bool IsFileSystem(int sysno);
  static bool IsAllowedFileSystemAccessViaFd(int sysno);
  static bool IsDeniedFileSystemAccessViaFd(int sysno);
  static bool IsGetSimpleId(int sysno);
  static bool IsProcessPrivilegeChange(int sysno);
  static bool IsProcessGroupOrSession(int sysno);
  static bool IsAllowedSignalHandling(int sysno);
  static bool IsAllowedOperationOnFd(int sysno);
  static bool IsKernelInternalApi(int sysno);
  
  static bool IsAllowedProcessStartOrDeath(int sysno);
  
  static bool IsFutex(int sysno);
  static bool IsAllowedEpoll(int sysno);
  static bool IsAllowedGetOrModifySocket(int sysno);
  static bool IsDeniedGetOrModifySocket(int sysno);

#if defined(__i386__)
  
  static bool IsSocketCall(int sysno);
#endif

#if defined(__x86_64__) || defined(__arm__)
  static bool IsNetworkSocketInformation(int sysno);
#endif

  static bool IsAllowedAddressSpaceAccess(int sysno);
  static bool IsAllowedGeneralIo(int sysno);
  static bool IsAllowedPrctl(int sysno);
  static bool IsAllowedBasicScheduler(int sysno);
  static bool IsAdminOperation(int sysno);
  static bool IsKernelModule(int sysno);
  static bool IsGlobalFSViewChange(int sysno);
  static bool IsFsControl(int sysno);
  static bool IsNuma(int sysno);
  static bool IsMessageQueue(int sysno);
  static bool IsGlobalProcessEnvironment(int sysno);
  static bool IsDebug(int sysno);
  static bool IsGlobalSystemStatus(int sysno);
  static bool IsEventFd(int sysno);
  
  static bool IsAsyncIo(int sysno);
  static bool IsKeyManagement(int sysno);
#if defined(__x86_64__) || defined(__arm__)
  static bool IsSystemVSemaphores(int sysno);
#endif
#if defined(__x86_64__) || defined(__arm__)
  
  static bool IsSystemVSharedMemory(int sysno);
#endif

#if defined(__x86_64__) || defined(__arm__)
  static bool IsSystemVMessageQueue(int sysno);
#endif

#if defined(__i386__)
  
  static bool IsSystemVIpc(int sysno);
#endif

  static bool IsAnySystemV(int sysno);
  static bool IsAdvancedScheduler(int sysno);
  static bool IsInotify(int sysno);
  static bool IsFaNotify(int sysno);
  static bool IsTimer(int sysno);
  static bool IsAdvancedTimer(int sysno);
  static bool IsExtendedAttributes(int sysno);
  static bool IsMisc(int sysno);
#if defined(__arm__)
  static bool IsArmPciConfig(int sysno);
  static bool IsArmPrivate(int sysno);
#endif  
 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(SyscallSets);
};

}  

#endif  