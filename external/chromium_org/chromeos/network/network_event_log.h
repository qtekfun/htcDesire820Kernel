// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_NETWORK_EVENT_LOG_H_
#define CHROMEOS_NETWORK_NETWORK_EVENT_LOG_H_

#include <deque>
#include <string>

#include "base/basictypes.h"
#include "base/strings/stringprintf.h"
#include "base/time/time.h"
#include "chromeos/chromeos_export.h"

namespace base {
class Value;
}

namespace chromeos {

namespace network_event_log {

enum StringOrder {
  OLDEST_FIRST,
  NEWEST_FIRST
};

enum LogLevel {
  LOG_LEVEL_ERROR = 0,
  LOG_LEVEL_USER = 1,
  LOG_LEVEL_EVENT = 2,
  LOG_LEVEL_DEBUG = 3
};

CHROMEOS_EXPORT extern const LogLevel kDefaultLogLevel;

CHROMEOS_EXPORT void Initialize();
CHROMEOS_EXPORT void Shutdown();

CHROMEOS_EXPORT bool IsInitialized();

namespace internal {

CHROMEOS_EXPORT size_t GetMaxLogEntries();

CHROMEOS_EXPORT void SetMaxLogEntries(size_t max_entries);

CHROMEOS_EXPORT void AddEntry(const char* file,
                              int file_line,
                              LogLevel log_level,
                              const std::string& event,
                              const std::string& description);

}  

CHROMEOS_EXPORT std::string GetAsString(StringOrder order,
                                        const std::string& format,
                                        LogLevel max_level,
                                        size_t max_events);

CHROMEOS_EXPORT std::string ValueAsString(const base::Value& value);

#define NET_LOG_ERROR(event, desc) NET_LOG_LEVEL(                       \
    ::chromeos::network_event_log::LOG_LEVEL_ERROR, event, desc)

#define NET_LOG_USER(event, desc) NET_LOG_LEVEL(                        \
    ::chromeos::network_event_log::LOG_LEVEL_USER, event, desc)

#define NET_LOG_EVENT(event, desc) NET_LOG_LEVEL(                       \
    ::chromeos::network_event_log::LOG_LEVEL_EVENT, event, desc)

#define NET_LOG_DEBUG(event, desc) NET_LOG_LEVEL(                       \
    ::chromeos::network_event_log::LOG_LEVEL_DEBUG, event, desc)

#define NET_LOG_LEVEL(log_level, event, description)            \
  ::chromeos::network_event_log::internal::AddEntry(            \
      __FILE__, __LINE__, log_level, event, description)

}  

}  

#endif  
