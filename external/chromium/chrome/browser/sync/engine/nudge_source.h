// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_NUDGE_SOURCE_H_
#define CHROME_BROWSER_SYNC_ENGINE_NUDGE_SOURCE_H_
#pragma once

namespace browser_sync {

enum NudgeSource {
  NUDGE_SOURCE_UNKNOWN = 0,
  
  NUDGE_SOURCE_NOTIFICATION,
  
  NUDGE_SOURCE_LOCAL,
  
  NUDGE_SOURCE_CONTINUATION,
};

}  

#endif  
