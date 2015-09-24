// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_SESSIONS_SESSION_TYPES_TEST_HELPER_H_
#define COMPONENTS_SESSIONS_SESSION_TYPES_TEST_HELPER_H_

#include <string>

#include "base/basictypes.h"
#include "content/public/common/page_transition_types.h"

class GURL;

namespace base {
class Time;
}

namespace content {
class PageState;
struct Referrer;
}

namespace sessions {

class SerializedNavigationEntry;

class SerializedNavigationEntryTestHelper {
 public:
  
  
  static void ExpectNavigationEquals(const SerializedNavigationEntry& expected,
                                     const SerializedNavigationEntry& actual);

  
  
  static SerializedNavigationEntry CreateNavigation(
      const std::string& virtual_url,
      const std::string& title);

  static void SetPageState(const content::PageState& page_state,
                           SerializedNavigationEntry* navigation);

  static void SetHasPostData(bool has_post_data,
                             SerializedNavigationEntry* navigation);

  static void SetOriginalRequestURL(const GURL& original_request_url,
                                    SerializedNavigationEntry* navigation);

  static void SetIsOverridingUserAgent(bool is_overriding_user_agent,
                                       SerializedNavigationEntry* navigation);

  static void SetTimestamp(base::Time timestamp,
                           SerializedNavigationEntry* navigation);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(SerializedNavigationEntryTestHelper);
};

}  

#endif  
