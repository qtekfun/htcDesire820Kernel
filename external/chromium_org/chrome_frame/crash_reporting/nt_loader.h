// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_FRAME_NT_LOADER_H_
#define CHROME_FRAME_NT_LOADER_H_

#include <windows.h>
#include <winnt.h>
#include <winternl.h>

namespace nt_loader {

struct _PEB;
struct _PEB_LDR_DATA;
struct _RTL_USER_PROCESS_PARAMETERS;
struct _PEB_FREE_BLOCK;


typedef struct _NT_TIB {
  _EXCEPTION_REGISTRATION_RECORD* ExceptionList;  
  void *StackBase;  
  void* StackLimit;  
  void* SubSystemTib;  
  union {
    void *FiberData;  
    DWORD Version;  
  };
  void* ArbitraryUserPointer;  
  _NT_TIB* Self;  
} _NT_TIB, NT_TIB;

typedef struct _CLIENT_ID {
  void* UniqueProcess;  
  void* UniqueThread;  
} _CLIENT_ID, CLIENT_ID;

typedef struct _TEB {
  _NT_TIB NtTib;  
  void* EnvironmentPointer;  
  _CLIENT_ID ClientId;  
  void* ActiveRpcHandle;  
  void* ThreadLocalStoragePointer;  
  _PEB* ProcessEnvironmentBlock;  
  
} _TEB, TEB;

typedef struct _PEB {
  BYTE InheritedAddressSpace;  
  BYTE ReadImageFileExecOptions;  
  BYTE BeingDebugged;  
  BYTE SpareBool;  
  void* Mutant;  
  void* ImageBaseAddress;  
  _PEB_LDR_DATA* Ldr;  
  _RTL_USER_PROCESS_PARAMETERS* ProcessParameters;  
  void* SubSystemData;  
  void* ProcessHeap;  
  _RTL_CRITICAL_SECTION* FastPebLock;  
  void* FastPebLockRoutine;  
  void* FastPebUnlockRoutine;  
  ULONG EnvironmentUpdateCount;  
  void* KernelCallbackTable;  
  ULONG SystemReserved[1]; 
  ULONG AtlThunkSListPtr32;  
  _PEB_FREE_BLOCK* FreeList;  
  ULONG TlsExpansionCounter;  
  void* TlsBitmap;  
  ULONG TlsBitmapBits[2]; 
  void* ReadOnlySharedMemoryBase;  
  void* ReadOnlySharedMemoryHeap;  
  void** ReadOnlyStaticServerData;  
  void* AnsiCodePageData;  
  void* OemCodePageData;  
  void* UnicodeCaseTableData;  
  ULONG NumberOfProcessors;  
  ULONG NtGlobalFlag;  
  _LARGE_INTEGER CriticalSectionTimeout;  
  ULONG HeapSegmentReserve;  
  ULONG HeapSegmentCommit;  
  ULONG HeapDeCommitTotalFreeThreshold;  
  ULONG HeapDeCommitFreeBlockThreshold;  
  ULONG NumberOfHeaps;  
  ULONG MaximumNumberOfHeaps;  
  void** ProcessHeaps;  
  void* GdiSharedHandleTable;  
  void* ProcessStarterHelper;  
  ULONG GdiDCAttributeList;  
  RTL_CRITICAL_SECTION* LoaderLock;  
  
} _PEB, PEB;

struct _PEB_LDR_DATA {
  ULONG Length;  
  BYTE Initialized;  
  void* SsHandle;  
  LIST_ENTRY InLoadOrderModuleList;  
  LIST_ENTRY InMemoryOrderModuleList;  
  LIST_ENTRY InInitializationOrderModuleList;  
  
};

#define LDRP_STATIC_LINK            0x00000002
#define LDRP_IMAGE_DLL              0x00000004
#define LDRP_LOAD_IN_PROGRESS       0x00001000
#define LDRP_UNLOAD_IN_PROGRESS     0x00002000
#define LDRP_ENTRY_PROCESSED        0x00004000
#define LDRP_DONT_CALL_FOR_THREADS  0x00040000
#define LDRP_PROCESS_ATTACH_CALLED  0x00080000
#define LDRP_COR_IMAGE              0x00400000
#define LDRP_COR_OWNS_UNMAP         0x00800000
#define LDRP_COR_IL_ONLY            0x01000000
#define LDRP_REDIRECTED             0x10000000

typedef struct _LDR_DATA_TABLE_ENTRY {
  LIST_ENTRY InLoadOrderLinks;  
  LIST_ENTRY InMemoryOrderLinks;  
  LIST_ENTRY InInitializationOrderLinks; 
  void* DllBase;  
  void* EntryPoint;  
  ULONG SizeOfImage;  
  UNICODE_STRING FullDllName;  
  UNICODE_STRING BaseDllName;  
  ULONG Flags;  
  USHORT LoadCount;  
  USHORT TlsIndex;  
  union {
    LIST_ENTRY HashLinks;  
    struct {
      void* SectionPointer;  
      ULONG CheckSum;  
    };
  };
  union {
    ULONG TimeDateStamp;  
    void* LoadedImports;  
  };
  void *EntryPointActivationContext;  
  void* PatchInformation;  
} _LDR_DATA_TABLE_ENTRY, LDR_DATA_TABLE_ENTRY;

inline TEB* GetCurrentTeb() {
  return reinterpret_cast<TEB*>(NtCurrentTeb());
}

inline PEB* GetCurrentPeb() {
  return GetCurrentTeb()->ProcessEnvironmentBlock;
}

inline bool OwnsCriticalSection(CRITICAL_SECTION* critsec) {
  return reinterpret_cast<DWORD>(critsec->OwningThread) ==
      GetCurrentThreadId();
}

LDR_DATA_TABLE_ENTRY* GetLoaderEntry(HMODULE module);

inline CRITICAL_SECTION* GetLoaderLock() {
  return GetCurrentPeb()->LoaderLock;
}

inline bool OwnsLoaderLock() {
  return OwnsCriticalSection(GetLoaderLock());
}

}  

#endif  
