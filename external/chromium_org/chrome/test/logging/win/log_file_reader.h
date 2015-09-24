// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_TEST_LOGGING_WIN_LOG_FILE_READER_H_
#define CHROME_TEST_LOGGING_WIN_LOG_FILE_READER_H_

#include <stddef.h>
#include <windows.h>
#include <wmistr.h>
#include <evntrace.h>

#include "base/logging.h"
#include "base/strings/string_piece.h"

namespace base {
class FilePath;
}

namespace logging_win {

class LogFileDelegate {
 public:
  virtual ~LogFileDelegate();

  
  virtual void OnUnknownEvent(const EVENT_TRACE* event) = 0;

  
  
  virtual void OnUnparsableEvent(const EVENT_TRACE* event) = 0;

  
  virtual void OnFileHeader(const EVENT_TRACE* event,
                            const TRACE_LOGFILE_HEADER* header) = 0;

  
  virtual void OnLogMessage(const EVENT_TRACE* event,
                            logging::LogSeverity severity,
                            const base::StringPiece& message) = 0;

  
  virtual void OnLogMessageFull(const EVENT_TRACE* event,
                                logging::LogSeverity severity,
                                DWORD stack_depth,
                                const intptr_t* backtrace,
                                int line,
                                const base::StringPiece& file,
                                const base::StringPiece& message) = 0;

  
  virtual void OnTraceEvent(const EVENT_TRACE* event,
                            const base::StringPiece& name,
                            char type,
                            intptr_t id,
                            const base::StringPiece& extra,
                            DWORD stack_depth,
                            const intptr_t* backtrace) = 0;

 protected:
  LogFileDelegate();
};

void ReadLogFile(const base::FilePath& log_file, LogFileDelegate* delegate);

}  

#endif  
