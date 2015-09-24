// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_NAVIGATION_DETAILS_H_
#define CONTENT_PUBLIC_BROWSER_NAVIGATION_DETAILS_H_

#include <string>
#include "content/common/content_export.h"
#include "content/public/browser/navigation_type.h"
#include "url/gurl.h"

namespace content {

class NavigationEntry;

struct CONTENT_EXPORT LoadCommittedDetails {
  
  
  LoadCommittedDetails();

  
  NavigationEntry* entry;

  
  
  
  content::NavigationType type;

  
  
  int previous_entry_index;

  
  GURL previous_url;

  
  
  bool did_replace_entry;

  
  
  bool is_in_page;

  
  
  bool is_main_frame;

  
  
  
  
  std::string serialized_security_info;

  
  
  
  bool is_navigation_to_different_page() const {
    return is_main_frame && !is_in_page;
  }

  
  int http_status_code;
};

struct EntryChangedDetails {
  
  const NavigationEntry* changed_entry;

  
  int index;
};

struct PrunedDetails {
  
  
  bool from_front;

  
  int count;
};

}  

#endif  
