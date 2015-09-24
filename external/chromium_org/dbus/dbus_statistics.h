// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_DBUS_STATISTICS_H_
#define DBUS_DBUS_STATISTICS_H_

#include <string>

#include "dbus/dbus_export.h"


namespace dbus {
namespace statistics {

enum ShowInString {
  SHOW_SERVICE = 0,  
  SHOW_INTERFACE = 1,  
  SHOW_METHOD = 2,  
};

enum FormatString {
  FORMAT_TOTALS = 0,  
  FORMAT_PER_MINUTE = 1,  
  FORMAT_ALL = 2  
};

CHROME_DBUS_EXPORT void Initialize();
CHROME_DBUS_EXPORT void Shutdown();

CHROME_DBUS_EXPORT void AddSentMethodCall(const std::string& service,
                                          const std::string& interface,
                                          const std::string& method);
CHROME_DBUS_EXPORT void AddReceivedSignal(const std::string& service,
                                          const std::string& interface,
                                          const std::string& method);
CHROME_DBUS_EXPORT void AddBlockingSentMethodCall(const std::string& service,
                                                  const std::string& interface,
                                                  const std::string& method);

CHROME_DBUS_EXPORT std::string GetAsString(ShowInString show,
                                           FormatString format);

namespace testing {
CHROME_DBUS_EXPORT bool GetCalls(const std::string& service,
                                 const std::string& interface,
                                 const std::string& method,
                                 int* sent,
                                 int* received,
                                 int* blocking);
}  

}  
}  

#endif  
