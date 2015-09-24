// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_CRASH_REPORTING_VECTORED_HANDLER_IMPL_H_
#define CHROME_FRAME_CRASH_REPORTING_VECTORED_HANDLER_IMPL_H_

#include "base/logging.h"
#include "chrome_frame/crash_reporting/vectored_handler.h"
#include "chrome_frame/crash_reporting/nt_loader.h"

#if !defined(_M_IX86)
#error only x86 is supported for now.
#endif

#ifndef EXCEPTION_CHAIN_END
#define EXCEPTION_CHAIN_END ((struct _EXCEPTION_REGISTRATION_RECORD*)-1)
#if !defined(_WIN32_WINNT_WIN8)
typedef struct _EXCEPTION_REGISTRATION_RECORD {
  struct _EXCEPTION_REGISTRATION_RECORD* Next;
  PVOID Handler;
} EXCEPTION_REGISTRATION_RECORD;
#define EXCEPTION_UNWINDING 0x2         
#define EXCEPTION_EXIT_UNWIND 0x4       
#define EXCEPTION_STACK_INVALID 0x8     
#define EXCEPTION_NESTED_CALL 0x10      
#define EXCEPTION_TARGET_UNWIND 0x20    
#define EXCEPTION_COLLIDED_UNWIND 0x40  

#define EXCEPTION_UNWIND (EXCEPTION_UNWINDING | EXCEPTION_EXIT_UNWIND | \
    EXCEPTION_TARGET_UNWIND | EXCEPTION_COLLIDED_UNWIND)

#define IS_UNWINDING(Flag)  (((Flag) & EXCEPTION_UNWIND) != 0)
#define IS_DISPATCHING(Flag)  (((Flag) & EXCEPTION_UNWIND) == 0)
#define IS_TARGET_UNWIND(Flag)  ((Flag) & EXCEPTION_TARGET_UNWIND)
#endif  
#endif  

template <typename E>
VectoredHandlerT<E>::VectoredHandlerT(E* api) : exceptions_seen_(0), api_(api) {
}

template <typename E>
VectoredHandlerT<E>::~VectoredHandlerT() {
}

template <typename E>
LONG VectoredHandlerT<E>::Handler(EXCEPTION_POINTERS* exceptionInfo) {
  
  const DWORD exceptionCode = exceptionInfo->ExceptionRecord->ExceptionCode;

  
  
  
  
  
  
  
  
  
  if ((exceptionCode & ERROR_SEVERITY_ERROR) != ERROR_SEVERITY_ERROR) {
    return ExceptionContinueSearch;
  }

  
  
  
  if (exceptionCode & APPLICATION_ERROR_MASK) {
    return ExceptionContinueSearch;
  }

  exceptions_seen_++;

  
  
  
  
  if (exceptionCode != STATUS_STACK_OVERFLOW && api_->CheckForStackOverflow()) {
    return ExceptionContinueSearch;
  }

  
  
  const EXCEPTION_REGISTRATION_RECORD* seh = api_->RtlpGetExceptionList();
  if (api_->ShouldIgnoreException(exceptionInfo, seh)) {
    return ExceptionContinueSearch;
  }

  const DWORD exceptionFlags = exceptionInfo->ExceptionRecord->ExceptionFlags;
  
  if (IS_DISPATCHING(exceptionFlags)) {
    if (ModuleHasInstalledSEHFilter())
      return ExceptionContinueSearch;

    if (api_->IsOurModule(exceptionInfo->ExceptionRecord->ExceptionAddress)) {
      api_->WriteDump(exceptionInfo);
      return ExceptionContinueSearch;
    }

    
    void* back_trace[E::max_back_trace] = {0};
    DWORD captured = api_->RtlCaptureStackBackTrace(0, api_->max_back_trace,
                                                    &back_trace[0], NULL);
    for (DWORD i = 0; i < captured; ++i) {
      if (api_->IsOurModule(back_trace[i])) {
        api_->WriteDump(exceptionInfo);
        return ExceptionContinueSearch;
      }
    }
  }

  return ExceptionContinueSearch;
}

template <typename E>
bool VectoredHandlerT<E>::ModuleHasInstalledSEHFilter() {
  const EXCEPTION_REGISTRATION_RECORD* RegistrationFrame =
      api_->RtlpGetExceptionList();
  
  
  while (RegistrationFrame != EXCEPTION_CHAIN_END) {
    if (api_->IsOurModule(RegistrationFrame->Handler)) {
      return true;
    }

    RegistrationFrame = RegistrationFrame->Next;
  }

  return false;
}


namespace {
  __declspec(naked)
    static EXCEPTION_REGISTRATION_RECORD* InternalRtlpGetExceptionList() {
      __asm {
        mov eax, fs:0
        ret
      }
  }

  __declspec(naked)
  static char* GetStackTopLimit() {
    __asm {
      mov eax, fs:8
      ret
    }
  }
}  

class Win32VEHTraits {
 public:
  enum {max_back_trace = 62};

  static inline
  EXCEPTION_REGISTRATION_RECORD* RtlpGetExceptionList() {
    return InternalRtlpGetExceptionList();
  }

  static inline WORD RtlCaptureStackBackTrace(DWORD FramesToSkip,
      DWORD FramesToCapture, void** BackTrace, DWORD* BackTraceHash) {
    return ::RtlCaptureStackBackTrace(FramesToSkip, FramesToCapture,
                                      BackTrace, BackTraceHash);
  }

  static bool ShouldIgnoreException(const EXCEPTION_POINTERS* exceptionInfo,
      const EXCEPTION_REGISTRATION_RECORD* seh_record) {
    const void* address = exceptionInfo->ExceptionRecord->ExceptionAddress;
    const DWORD flags = GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT |
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS;
    HMODULE crashing_module = GetModuleHandleFromAddress(address);
    if (!crashing_module)
      return false;

    HMODULE top_seh_module = NULL;
    if (EXCEPTION_CHAIN_END != seh_record)
      top_seh_module = GetModuleHandleFromAddress(seh_record->Handler);

    
    
    
    if (crashing_module == top_seh_module)
      return true;

    
    
    
    
    
    if (nt_loader::OwnsLoaderLock()) {
      nt_loader::LDR_DATA_TABLE_ENTRY* entry =
          nt_loader::GetLoaderEntry(crashing_module);

      
      
      
      
      
      
      
      
      
      if (entry &&
          (entry->Flags & LDRP_IMAGE_DLL) != 0 &&
          (entry->EntryPoint != NULL) &&
          (entry->Flags & LDRP_PROCESS_ATTACH_CALLED) == 0)
        return true;
    }

    return false;
  }

  static bool CheckForStackOverflow() {
    MEMORY_BASIC_INFORMATION mi = {0};
    const DWORD kPageSize = 0x1000;
    void* stack_top = GetStackTopLimit() - kPageSize;
    ::VirtualQuery(stack_top, &mi, sizeof(mi));
    
    
    void* stack_top2 = GetStackTopLimit() - kPageSize;
    if (stack_top2 != stack_top)
      ::VirtualQuery(stack_top2, &mi, sizeof(mi));
    return !(mi.Protect & PAGE_GUARD);
  }

 private:
  static inline const void* CodeOffset(const void* code, int offset) {
    return reinterpret_cast<const char*>(code) + offset;
  }

  static HMODULE GetModuleHandleFromAddress(const void* p) {
    HMODULE module_at_address = NULL;
    MEMORY_BASIC_INFORMATION mi = {0};
    if (::VirtualQuery(p, &mi, sizeof(mi)) && (mi.Type & MEM_IMAGE)) {
      module_at_address = reinterpret_cast<HMODULE>(mi.AllocationBase);
    }

    return module_at_address;
  }
};


class CrashHandlerTraits : public Win32VEHTraits,
                           public ModuleOfInterestWithExcludedRegion {
 public:

  typedef bool (*DumpHandler)(EXCEPTION_POINTERS* p);

  CrashHandlerTraits() : dump_handler_(NULL) {}

  
  void Init(const void* veh_segment_start, const void* veh_segment_end,
            DumpHandler dump_handler) {
    DCHECK(dump_handler);
    dump_handler_ = dump_handler;
    ModuleOfInterestWithExcludedRegion::SetCurrentModule();
    
    
    
    
    ModuleOfInterestWithExcludedRegion::SetExcludedRegion(veh_segment_start,
                                                          veh_segment_end);
  }

  void Shutdown() {
  }

  inline bool WriteDump(EXCEPTION_POINTERS* p) {
    if (dump_handler_) {
      return dump_handler_(p);
    } else {
      return false;
    }
  }

 private:
  DumpHandler dump_handler_;
};

#endif  
