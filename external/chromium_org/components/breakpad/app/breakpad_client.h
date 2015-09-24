// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BREAKPAD_APP_BREAKPAD_CLIENT_H_
#define COMPONENTS_BREAKPAD_APP_BREAKPAD_CLIENT_H_

#include <string>

#include "base/strings/string16.h"
#include "build/build_config.h"

namespace base {
class FilePath;
}

#if defined(OS_MACOSX)
typedef void* BreakpadRef;
#endif

namespace breakpad {

class BreakpadClient;

void SetBreakpadClient(BreakpadClient* client);

#if defined(BREAKPAD_IMPLEMENTATION)
BreakpadClient* GetBreakpadClient();
#endif

class BreakpadClient {
 public:
  BreakpadClient();
  virtual ~BreakpadClient();

  
  
  virtual void SetClientID(const std::string& client_id);

#if defined(OS_WIN)
  
  
  virtual bool GetAlternativeCrashDumpLocation(base::FilePath* crash_dir);

  
  
  virtual void GetProductNameAndVersion(const base::FilePath& exe_path,
                                        base::string16* product_name,
                                        base::string16* version,
                                        base::string16* special_build,
                                        base::string16* channel_name);

  
  
  
  
  virtual bool ShouldShowRestartDialog(base::string16* title,
                                       base::string16* message,
                                       bool* is_rtl_locale);

  
  
  virtual bool AboutToRestart();

  
  virtual bool GetDeferredUploadsSupported(bool is_per_user_install);

  
  virtual bool GetIsPerUserInstall(const base::FilePath& exe_path);

  
  virtual bool GetShouldDumpLargerDumps(bool is_per_user_install);

  
  
  virtual int GetResultCodeRespawnFailed();

  
  virtual void InitBrowserCrashDumpsRegKey();

  
  virtual void RecordCrashDumpAttempt(bool is_real_crash);
#endif

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_IOS)
  
  
  virtual void GetProductNameAndVersion(std::string* product_name,
                                        std::string* version);

  virtual base::FilePath GetReporterLogFilename();
#endif

  // The location where minidump files should be written. Returns true if
  
  virtual bool GetCrashDumpLocation(base::FilePath* crash_dir);

#if defined(OS_POSIX)
  
  
  virtual void SetDumpWithoutCrashingFunction(void (*function)());
#endif

  
  
  virtual size_t RegisterCrashKeys();

  
  virtual bool IsRunningUnattended();

  
  virtual bool GetCollectStatsConsent();

#if defined(OS_WIN) || defined(OS_MACOSX)
  
  
  virtual bool ReportingIsEnforcedByPolicy(bool* breakpad_enabled);
#endif

#if defined(OS_ANDROID)
  
  virtual int GetAndroidMinidumpDescriptor();
#endif

#if defined(OS_MACOSX)
  
  virtual void InstallAdditionalFilters(BreakpadRef breakpad);
#endif

  
  virtual bool EnableBreakpadForProcess(const std::string& process_type);
};

}  

#endif  
