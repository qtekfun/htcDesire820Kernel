// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_GOOGLETEST_H__
#define GOOGLE_PROTOBUF_GOOGLETEST_H__

#include <map>
#include <vector>
#include <google/protobuf/stubs/common.h>

#if !PROTOBUF_USE_EXCEPTIONS && defined(GTEST_HAS_DEATH_TEST)
#define PROTOBUF_HAS_DEATH_TEST
#endif

namespace google {
namespace protobuf {

string TestSourceDir();

string TestTempDir();

// Capture all text written to stdout or stderr.
void CaptureTestStdout();
void CaptureTestStderr();

string GetCapturedTestStdout();
string GetCapturedTestStderr();

#undef ERROR  
static const LogLevel ERROR = LOGLEVEL_ERROR;
static const LogLevel WARNING = LOGLEVEL_WARNING;

class ScopedMemoryLog {
 public:
  ScopedMemoryLog();
  virtual ~ScopedMemoryLog();

  
  const vector<string>& GetMessages(LogLevel error);

 private:
  map<LogLevel, vector<string> > messages_;
  LogHandler* old_handler_;

  static void HandleLog(LogLevel level, const char* filename, int line,
                        const string& message);

  static ScopedMemoryLog* active_log_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ScopedMemoryLog);
};

}  
}  

#endif  
