// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_USAGE_STATS_CONSENT_H_
#define REMOTING_HOST_USAGE_STATS_CONSENT_H_

namespace remoting {

bool GetUsageStatsConsent(bool* allowed, bool* set_by_policy);

bool IsUsageStatsAllowed();

bool SetUsageStatsConsent(bool allowed);

}  

#endif  
