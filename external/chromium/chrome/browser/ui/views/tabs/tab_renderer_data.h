// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_TAB_RENDERER_DATA_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_TAB_RENDERER_DATA_H_
#pragma once

#include "base/process_util.h"
#include "base/string16.h"
#include "googleurl/src/gurl.h"
#include "third_party/skia/include/core/SkBitmap.h"

struct TabRendererData {
  
  
  
  enum NetworkState {
    NETWORK_STATE_NONE,     
    NETWORK_STATE_WAITING,  
    NETWORK_STATE_LOADING,  
  };

  TabRendererData();
  ~TabRendererData();

  
  
  
  bool IsCrashed() const {
    return (crashed_status == base::TERMINATION_STATUS_PROCESS_WAS_KILLED ||
            crashed_status == base::TERMINATION_STATUS_PROCESS_CRASHED ||
            crashed_status == base::TERMINATION_STATUS_ABNORMAL_TERMINATION);
  }

  
  
  bool Equals(const TabRendererData& data);

  SkBitmap favicon;
  NetworkState network_state;
  string16 title;
  GURL url;
  bool loading;
  base::TerminationStatus crashed_status;
  bool incognito;
  bool show_icon;
  bool mini;
  bool blocked;
  bool app;
};

#endif  
