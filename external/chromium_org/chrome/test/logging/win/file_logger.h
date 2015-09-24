// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_LOGGING_WIN_FILE_LOGGER_H_
#define CHROME_TEST_LOGGING_WIN_FILE_LOGGER_H_

#include <guiddef.h>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "base/win/event_trace_controller.h"

namespace base {
class FilePath;
}

namespace logging_win {

class FileLogger {
 public:
  enum EventProviderBits {
    
    CHROME_LOG_PROVIDER         = 1 << 0,
    
    CHROME_FRAME_LOG_PROVIDER   = 1 << 1,
    
    CHROME_TESTS_LOG_PROVIDER   = 1 << 2,
    
    CHROME_TRACE_EVENT_PROVIDER = 1 << 3,
  };

  static const uint32 kAllEventProviders = (CHROME_LOG_PROVIDER |
                                            CHROME_FRAME_LOG_PROVIDER |
                                            CHROME_TESTS_LOG_PROVIDER |
                                            CHROME_TRACE_EVENT_PROVIDER);

  FileLogger();
  ~FileLogger();

  
  void Initialize();

  
  
  void Initialize(uint32 event_provider_mask);

  
  
  
  
  bool StartLogging(const base::FilePath& log_file);

  
  void StopLogging();

  
  bool is_logging() const {
    return controller_.session_name() && *controller_.session_name();
  }

 private:
  bool EnableProviders();
  void DisableProviders();

  static bool is_initialized_;

  base::win::EtwTraceController controller_;
  uint32 event_provider_mask_;

  DISALLOW_COPY_AND_ASSIGN(FileLogger);
};

}  

#endif  
