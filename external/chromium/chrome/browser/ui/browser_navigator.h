// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_NAVIGATOR_H_
#define CHROME_BROWSER_UI_BROWSER_NAVIGATOR_H_
#pragma once

#include <string>

#include "content/common/page_transition_types.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/rect.h"
#include "webkit/glue/window_open_disposition.h"

class Browser;
class Profile;
class TabContentsWrapper;

namespace browser {

struct NavigateParams {
  NavigateParams(Browser* browser,
                 const GURL& a_url,
                 PageTransition::Type a_transition);
  NavigateParams(Browser* browser, TabContentsWrapper* a_target_contents);
  ~NavigateParams();

  
  GURL url;
  GURL referrer;

  
  
  
  
  
  
  
  
  
  
  
  
  TabContentsWrapper* target_contents;

  
  
  
  
  
  
  
  TabContentsWrapper* source_contents;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  WindowOpenDisposition disposition;

  
  
  PageTransition::Type transition;

  
  
  
  
  int tabstrip_index;

  
  
  
  int tabstrip_add_types;

  
  std::string extension_app_id;

  
  
  
  
  
  gfx::Rect window_bounds;

  
  
  enum WindowAction {
    
    NO_ACTION,
    
    SHOW_WINDOW,
    
    SHOW_WINDOW_INACTIVE
  };
  
  
  
  WindowAction window_action;

  
  enum PathBehavior {
    
    RESPECT,
    
    IGNORE_AND_NAVIGATE,
    
    IGNORE_AND_STAY_PUT,
  };
  
  PathBehavior path_behavior;

  
  
  
  
  
  
  
  
  
  
  
  
  Browser* browser;

  
  
  Profile* profile;

 private:
  NavigateParams();
};

void Navigate(NavigateParams* params);

}  

#endif  
