// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_LOGGER_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_LOGGER_H_

#include <string>
#include <vector>

#include "base/location.h"
#include "base/logging.h"
#include "chrome/browser/drive/event_logger.h"

namespace sync_file_system {
namespace util {

void ClearLog();

void Log(logging::LogSeverity level,
         const tracked_objects::Location& location,
         const char* format,
         ...)
    PRINTF_FORMAT(3, 4);

std::vector<drive::EventLogger::Event> GetLogHistory();

}  
}  

#endif  
