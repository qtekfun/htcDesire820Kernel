// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_INJECTION_TEST_WIN_H_
#define CONTENT_COMMON_INJECTION_TEST_WIN_H_


const char kRenderTestCall[] = "RunRendererTests";
const char kPluginTestCall[] = "RunPluginTests";

extern "C" {
#ifdef TEST_INJECTION_DLL
BOOL extern __declspec(dllexport) __cdecl RunRendererTests(int* test_count);
BOOL extern __declspec(dllexport) __cdecl RunPluginTests(int* test_count);
#else
typedef BOOL (__cdecl *RunRendererTests)(int* test_count);
typedef BOOL (__cdecl *RunPluginTests)(int* test_count);
#endif
}

#endif  
