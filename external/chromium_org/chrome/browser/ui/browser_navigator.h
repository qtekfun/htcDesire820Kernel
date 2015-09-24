// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_NAVIGATOR_H_
#define CHROME_BROWSER_UI_BROWSER_NAVIGATOR_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_memory.h"
#include "chrome/browser/ui/host_desktop.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/global_request_id.h"
#include "content/public/browser/page_navigator.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/common/referrer.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/rect.h"
#include "url/gurl.h"

class Browser;
class Profile;

namespace content {
class WebContents;
}

namespace chrome {

struct NavigateParams {
  NavigateParams(Browser* browser,
                 const GURL& a_url,
                 content::PageTransition a_transition);
  NavigateParams(Browser* browser,
                 content::WebContents* a_target_contents);
  NavigateParams(Profile* profile,
                 const GURL& a_url,
                 content::PageTransition a_transition);
  ~NavigateParams();

  
  GURL url;
  content::Referrer referrer;

  
  int64 frame_tree_node_id;

  
  
  std::vector<GURL> redirect_chain;

  
  
  
  
  bool uses_post;

  
  scoped_refptr<base::RefCountedMemory> browser_initiated_post_data;

  
  
  
  std::string extra_headers;

  
  
  
  
  
  
  
  
  
  
  
  
  content::WebContents* target_contents;

  
  
  
  
  
  
  
  content::WebContents* source_contents;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  WindowOpenDisposition disposition;

  
  
  
  content::PageTransition transition;

  
  
  bool is_renderer_initiated;

  
  
  
  
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

  
  
  bool user_gesture;

  
  enum PathBehavior {
    
    RESPECT,
    
    IGNORE_AND_NAVIGATE,
    
    IGNORE_AND_STAY_PUT,
  };
  
  PathBehavior path_behavior;

  
  enum RefBehavior {
    
    IGNORE_REF,
    
    RESPECT_REF,
  };
  
  RefBehavior ref_behavior;

  
  
  
  
  
  
  
  
  
  
  
  
  Browser* browser;

  
  
  Profile* initiating_profile;

  
  
  
  content::GlobalRequestID transferred_global_request_id;

  
  
  chrome::HostDesktopType host_desktop_type;

  
  
  bool should_replace_current_entry;

  
  
  bool should_set_opener;

 private:
  NavigateParams();
};

void FillNavigateParamsFromOpenURLParams(chrome::NavigateParams* nav_params,
                                         const content::OpenURLParams& params);

void Navigate(NavigateParams* params);

bool IsURLAllowedInIncognito(const GURL& url,
                             content::BrowserContext* browser_context);

}  

#endif  
