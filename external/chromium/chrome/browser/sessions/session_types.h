// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_SESSION_TYPES_H_
#define CHROME_BROWSER_SESSIONS_SESSION_TYPES_H_
#pragma once

#include <string>
#include <vector>

#include "base/stl_util-inl.h"
#include "base/string16.h"
#include "base/time.h"
#include "chrome/browser/sessions/session_id.h"
#include "content/common/page_transition_types.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/rect.h"

class NavigationEntry;
class Profile;


class TabNavigation {
 public:
  enum TypeMask {
    HAS_POST_DATA = 1
  };

  TabNavigation();
  TabNavigation(int index,
                const GURL& virtual_url,
                const GURL& referrer,
                const string16& title,
                const std::string& state,
                PageTransition::Type transition);
  TabNavigation(const TabNavigation& tab);
  ~TabNavigation();
  TabNavigation& operator=(const TabNavigation& tab);

  
  
  NavigationEntry* ToNavigationEntry(int page_id, Profile* profile) const;

  
  void SetFromNavigationEntry(const NavigationEntry& entry);

  
  void set_virtual_url(const GURL& url) { virtual_url_ = url; }
  const GURL& virtual_url() const { return virtual_url_; }

  
  const GURL& referrer() const { return referrer_; }

  
  const string16& title() const { return title_; }

  
  const std::string& state() const { return state_; }

  
  void set_transition(PageTransition::Type transition) {
    transition_ = transition;
  }
  PageTransition::Type transition() const { return transition_; }

  
  
  void set_type_mask(int type_mask) { type_mask_ = type_mask; }
  int type_mask() const { return type_mask_; }

  
  
  
  
  
  void set_index(int index) { index_ = index; }
  int index() const { return index_; }

 private:
  friend class BaseSessionService;

  GURL virtual_url_;
  GURL referrer_;
  string16 title_;
  std::string state_;
  PageTransition::Type transition_;
  int type_mask_;

  int index_;
};


struct SessionTab {
  SessionTab();
  ~SessionTab();

  
  SessionID window_id;

  
  SessionID tab_id;

  
  
  
  
  
  
  int tab_visual_index;

  
  
  
  
  
  
  
  
  
  
  int current_navigation_index;

  
  bool pinned;

  
  std::string extension_app_id;

  
  base::Time timestamp;

  std::vector<TabNavigation> navigations;

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

  
  bool is_maximized;

 private:
  DISALLOW_COPY_AND_ASSIGN(SessionWindow);
};

struct ForeignSession {
  ForeignSession();
  ~ForeignSession();

  
  std::string foreign_session_tag;
  std::vector<SessionWindow*> windows;
};

#endif  
