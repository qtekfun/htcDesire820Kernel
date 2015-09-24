// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CHILD_PROCESS_LOGGING_H_
#define CHROME_COMMON_CHILD_PROCESS_LOGGING_H_
#pragma once

#include <set>
#include <string>

#include "base/basictypes.h"
#include "googleurl/src/gurl.h"

struct GPUInfo;

#if defined(OS_WIN) || defined(OS_MACOSX)
static const int kMaxReportedActiveExtensions = 10;
#endif

namespace child_process_logging {

#if defined(OS_LINUX)
extern char g_active_url[];
extern char g_client_id[];
extern char g_gpu_vendor_id[];
extern char g_gpu_device_id[];
extern char g_gpu_driver_ver[];
extern char g_gpu_ps_ver[];
extern char g_gpu_vs_ver[];
#endif

void SetActiveURL(const GURL& url);

void SetClientId(const std::string& client_id);

std::string GetClientId();

void SetActiveExtensions(const std::set<std::string>& extension_ids);

void SetNumberOfViews(int number_of_views);

void SetGpuInfo(const GPUInfo& gpu_info);

class ScopedActiveURLSetter {
 public:
  explicit ScopedActiveURLSetter(const GURL& url)  {
    SetActiveURL(url);
  }

  ~ScopedActiveURLSetter()  {
    SetActiveURL(GURL());
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedActiveURLSetter);
};

}  

#if defined(OS_MACOSX) && __OBJC__

@class NSString;

typedef void (*SetCrashKeyValueFuncPtr)(NSString*, NSString*);
typedef void (*ClearCrashKeyValueFuncPtr)(NSString*);

namespace child_process_logging {
void SetCrashKeyFunctions(SetCrashKeyValueFuncPtr set_key_func,
                          ClearCrashKeyValueFuncPtr clear_key_func);
void SetActiveURLImpl(const GURL& url,
                      SetCrashKeyValueFuncPtr set_key_func,
                      ClearCrashKeyValueFuncPtr clear_key_func);

extern const int kMaxNumCrashURLChunks;
extern const int kMaxNumURLChunkValueLength;
extern const char *kUrlChunkFormatStr;
}  

#endif  

#endif  
