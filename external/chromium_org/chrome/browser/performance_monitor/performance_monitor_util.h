// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PERFORMANCE_MONITOR_PERFORMANCE_MONITOR_UTIL_H_
#define CHROME_BROWSER_PERFORMANCE_MONITOR_PERFORMANCE_MONITOR_UTIL_H_

#include "base/callback.h"
#include "base/time/time.h"
#include "base/tracked_objects.h"
#include "chrome/browser/performance_monitor/database.h"
#include "chrome/browser/performance_monitor/event.h"
#include "chrome/browser/performance_monitor/metric.h"
#include "chrome/common/extensions/extension_constants.h"

namespace performance_monitor {
namespace util {

bool PostTaskToDatabaseThreadAndReply(
    const tracked_objects::Location& from_here,
    const base::Closure& request,
    const base::Closure& reply);

scoped_ptr<Event> CreateExtensionEvent(const EventType type,
                                       const base::Time& time,
                                       const std::string& id,
                                       const std::string& name,
                                       const std::string& url,
                                       const int location,
                                       const std::string& version,
                                       const std::string& description);

scoped_ptr<Event> CreateRendererFailureEvent(const base::Time& time,
                                             const EventType& type,
                                             const std::string& url);

scoped_ptr<Event> CreateUncleanExitEvent(const base::Time& time,
                                         const std::string& profile_name);

scoped_ptr<Event> CreateChromeUpdateEvent(const base::Time& time,
                                          const std::string& previous_version,
                                          const std::string& current_version);

}  
}  

#endif  
