// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_WEBRTC_BROWSERTEST_PERF_H_
#define CHROME_BROWSER_MEDIA_WEBRTC_BROWSERTEST_PERF_H_

namespace base {
class DictionaryValue;
}

void PrintBweForVideoMetrics(const base::DictionaryValue& pc_dict);
void PrintMetricsForAllStreams(const base::DictionaryValue& pc_dict);

#endif  
