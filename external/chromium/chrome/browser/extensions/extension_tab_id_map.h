// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_TAB_ID_MAP_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_TAB_ID_MAP_H_
#pragma once

#include <map>
#include <utility>

#include "base/basictypes.h"
#include "base/memory/singleton.h"

class ExtensionTabIdMap {
 public:
  static ExtensionTabIdMap* GetInstance();

  
  
  void Init();
  void Shutdown();

  
  
  bool GetTabAndWindowId(
      int render_process_host_id, int routing_id, int* tab_id, int* window_id);

 private:
  class TabObserver;
  friend class TabObserver;
  friend struct DefaultSingletonTraits<ExtensionTabIdMap>;

  typedef std::pair<int, int> RenderId;
  typedef std::pair<int, int> TabAndWindowId;
  typedef std::map<RenderId, TabAndWindowId> TabAndWindowIdMap;

  ExtensionTabIdMap();
  ~ExtensionTabIdMap();

  
  void SetTabAndWindowId(
      int render_process_host_id, int routing_id, int tab_id, int window_id);
  void ClearTabAndWindowId(
      int render_process_host_id, int routing_id);

  TabObserver* observer_;
  TabAndWindowIdMap map_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionTabIdMap);
};

#endif  
