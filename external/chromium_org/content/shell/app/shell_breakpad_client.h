// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SHELL_APP_SHELL_BREAKPAD_CLIENT_H_
#define CONTENT_SHELL_APP_SHELL_BREAKPAD_CLIENT_H_

#include "base/compiler_specific.h"
#include "components/breakpad/app/breakpad_client.h"

namespace content {

class ShellBreakpadClient : public breakpad::BreakpadClient {
 public:
  ShellBreakpadClient();
  virtual ~ShellBreakpadClient();

#if defined(OS_WIN)
  
  
  virtual void GetProductNameAndVersion(const base::FilePath& exe_path,
                                        base::string16* product_name,
                                        base::string16* version,
                                        base::string16* special_build,
                                        base::string16* channel_name) OVERRIDE;
#endif

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_IOS)
  
  
  virtual void GetProductNameAndVersion(std::string* product_name,
                                        std::string* version) OVERRIDE;

  virtual base::FilePath GetReporterLogFilename() OVERRIDE;
#endif

  // The location where minidump files should be written. Returns true if
  
  virtual bool GetCrashDumpLocation(base::FilePath* crash_dir) OVERRIDE;

#if defined(OS_ANDROID)
  
  virtual int GetAndroidMinidumpDescriptor() OVERRIDE;
#endif

  virtual bool EnableBreakpadForProcess(
      const std::string& process_type) OVERRIDE;

private:
  DISALLOW_COPY_AND_ASSIGN(ShellBreakpadClient);
};

}  

#endif  
