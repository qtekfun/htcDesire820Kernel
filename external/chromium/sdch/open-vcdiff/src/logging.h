// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_LOGGING_H_
#define OPEN_VCDIFF_LOGGING_H_

#include <config.h>
#include <iostream>
#include <vector>

#ifdef ERROR
#undef ERROR
#endif  

namespace open_vcdiff {

enum LogLevel {
  INFO,
  WARNING,
  ERROR,
  FATAL
};

#ifndef NDEBUG
#define DFATAL FATAL
#else  
#define DFATAL ERROR
#endif  

extern bool g_fatal_error_occurred;
extern void (*ExitFatal)();

inline std::ostream& LogMessage(LogLevel level, const char* level_name) {
  if (level == FATAL) {
    g_fatal_error_occurred = true;
  }
  return std::cerr << level_name << ": ";
}

inline void CheckFatalError() {
  if (g_fatal_error_occurred) {
    g_fatal_error_occurred = false;
    (*ExitFatal)();
  }
}

}  

#define LOG(level)   LogMessage(open_vcdiff::level, #level)
#define LOG_ENDL     std::endl; \
                     open_vcdiff::CheckFatalError();

#endif  
