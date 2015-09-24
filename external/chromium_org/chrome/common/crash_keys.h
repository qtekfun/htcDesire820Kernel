// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CRASH_KEYS_H_
#define CHROME_COMMON_CRASH_KEYS_H_

#include <set>
#include <string>
#include <vector>

#include "base/debug/crash_logging.h"

class CommandLine;

namespace crash_keys {

size_t RegisterChromeCrashKeys();

void SetClientID(const std::string& client_id);

void SetSwitchesFromCommandLine(const CommandLine* command_line);

void SetVariationsList(const std::vector<std::string>& variations);

void SetActiveExtensions(const std::set<std::string>& extensions);

class ScopedPrinterInfo {
 public:
  explicit ScopedPrinterInfo(const base::StringPiece& data);
  ~ScopedPrinterInfo();

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedPrinterInfo);
};


extern const char kClientID[];

extern const char kChannel[];

extern const char kActiveURL[];

const size_t kSwitchesMaxCount = 15;
extern const char kSwitch[];
extern const char kNumSwitches[];

extern const char kNumVariations[];
extern const char kVariations[];

const size_t kExtensionIDMaxCount = 10;
extern const char kExtensionID[];
extern const char kNumExtensionsCount[];

extern const char kNumberOfViews[];

#if !defined(OS_ANDROID)
extern const char kGPUVendorID[];
extern const char kGPUDeviceID[];
#endif
extern const char kGPUDriverVersion[];
extern const char kGPUPixelShaderVersion[];
extern const char kGPUVertexShaderVersion[];
#if defined(OS_MACOSX)
extern const char kGPUGLVersion[];
#elif defined(OS_POSIX)
extern const char kGPUVendor[];
extern const char kGPURenderer[];
#endif

const size_t kPrinterInfoCount = 4;
extern const char kPrinterInfo[];

#if defined(OS_MACOSX)
namespace mac {

extern const char kFirstNSException[];
extern const char kFirstNSExceptionTrace[];

extern const char kLastNSException[];
extern const char kLastNSExceptionTrace[];

extern const char kNSException[];
extern const char kNSExceptionTrace[];

extern const char kSendAction[];

extern const char kZombie[];
extern const char kZombieTrace[];

}  
#endif

}  

#endif  
