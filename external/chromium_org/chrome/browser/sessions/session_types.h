// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_SESSION_TYPES_H_
#define CHROME_BROWSER_SESSIONS_SESSION_TYPES_H_

#include <algorithm>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "chrome/browser/sessions/session_id.h"
#include "components/sessions/serialized_navigation_entry.h"
#include "content/public/common/page_transition_types.h"
#include "sync/protocol/session_specifics.pb.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/rect.h"
#include "url/gurl.h"

namespace content {
class BrowserContext;
class NavigationEntry;
}


struct SessionTab {
  SessionTab();
  ~SessionTab();

  
  
  
  
  int normalized_navigation_index() const {
    return std::max(0, std::min(current_navigation_index,
                                static_cast<int>(navigations.size() - 1)));
  }

  
  
  
  
  void SetFromSyncData(const sync_pb::SessionTab& sync_data,
                       base::Time timestamp);

  
  
  
  
  
  sync_pb::SessionTab ToSyncData() const;

  
  SessionID window_id;

  
  SessionID tab_id;

  
  
  
  
  
  
  int tab_visual_index;

  
  
  
  
  
  
  
  
  
  
  int current_navigation_index;

  
  bool pinned;

  
  std::string extension_app_id;

  
  
  std::string user_agent_override;

  
  base::Time timestamp;

  std::vector<sessions::SerializedNavigationEntry> navigations;

  
  std::string session_storage_persistent_id;

 private:
  DISALLOW_COPY_AND_ASSIGN(SessionTab);
};


struct SessionWindow {
  SessionWindow();
  ~SessionWindow();

  
  SessionID window_id;

  
  gfx::Rect bounds;

  
  
  
  
  
  
  
  int selected_tab_index;

  
  
  
  int type;

  
  
  
  
  bool is_constrained;

  
  base::Time timestamp;

  
  std::vector<SessionTab*> tabs;

  
  ui::WindowShowState show_state;

  std::string app_name;

 private:
  DISALLOW_COPY_AND_ASSIGN(SessionWindow);
};

#endif  
