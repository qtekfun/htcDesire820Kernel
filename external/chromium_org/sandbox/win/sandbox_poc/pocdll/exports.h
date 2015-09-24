// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_SANDBOX_POC_POCDLL_EXPORTS_H__
#define SANDBOX_SANDBOX_POC_POCDLL_EXPORTS_H__

#include <windows.h>

#ifdef POCDLL_EXPORTS
#define POCDLL_API __declspec(dllexport) __cdecl
#else
#define POCDLL_API __declspec(dllimport) __cdecl
#endif

extern "C" {
void POCDLL_API TestFileSystem(HANDLE log);

void POCDLL_API TestGetHandle(HANDLE log);

void POCDLL_API TestThreadBombing(HANDLE log);

void POCDLL_API TestTakeAllCpu(HANDLE log);

void POCDLL_API TestUseAllMemory(HANDLE log);

void POCDLL_API TestCreateObjects(HANDLE log);

void POCDLL_API TestCloseHWND(HANDLE log);

void POCDLL_API TestNetworkListen(HANDLE log);

void POCDLL_API TestProcesses(HANDLE log);

void POCDLL_API TestThreads(HANDLE log);

void POCDLL_API TestRegistry(HANDLE log);

void POCDLL_API TestSpyKeys(HANDLE log);

void POCDLL_API TestSpyScreen(HANDLE log);

void POCDLL_API Run(HANDLE log);
}

#endif  
