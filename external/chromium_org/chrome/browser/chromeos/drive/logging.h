// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_LOGGING_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_LOGGING_H_

#include <vector>

#include "chrome/browser/drive/event_logger.h"

namespace drive {
namespace util {

void Log(
    logging::LogSeverity severity, const char* format, ...) PRINTF_FORMAT(2, 3);

std::vector<EventLogger::Event> GetLogHistory();

}  
}  

#endif  
