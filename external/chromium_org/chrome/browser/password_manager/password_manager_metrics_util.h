// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_MANAGER_METRICS_UTIL_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_MANAGER_METRICS_UTIL_H_

#include <string>

class PrefService;

namespace password_manager_metrics_util {


const size_t kGroupsPerDomain = 10u;

size_t MonitoredDomainGroupId(const std::string& url_host,
                              PrefService* pref_service);

void LogUMAHistogramEnumeration(const std::string& name,
                                int sample,
                                int boundary_value);

void LogUMAHistogramBoolean(const std::string& name, bool sample);

std::string GroupIdToString(size_t group_id);

}  

#endif  
