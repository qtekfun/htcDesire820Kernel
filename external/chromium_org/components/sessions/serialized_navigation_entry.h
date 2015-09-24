// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_SESSIONS_SERIALIZED_NAVIGATION_ENTRY_H_
#define COMPONENTS_SESSIONS_SERIALIZED_NAVIGATION_ENTRY_H_

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "components/sessions/sessions_export.h"
#include "content/public/common/page_state.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/common/referrer.h"
#include "url/gurl.h"

class Pickle;
class PickleIterator;

namespace content {
class BrowserContext;
class NavigationEntry;
}

namespace sync_pb {
class TabNavigation;
}

namespace sessions {

class SerializedNavigationEntryTestHelper;

SESSIONS_EXPORT extern const char kSearchTermsKey[];

class SESSIONS_EXPORT SerializedNavigationEntry {
 public:
  enum BlockedState {
    STATE_INVALID = 0,
    STATE_ALLOWED = 1,
    STATE_BLOCKED = 2,
  };

  
  SerializedNavigationEntry();
  ~SerializedNavigationEntry();

  
  
  static SerializedNavigationEntry FromNavigationEntry(
      int index,
      const content::NavigationEntry& entry);

  
  
  
  
  
  static SerializedNavigationEntry FromSyncData(
      int index,
      const sync_pb::TabNavigation& sync_data);

  
  
  void WriteToPickle(int max_size, Pickle* pickle) const;
  bool ReadFromPickle(PickleIterator* iterator);

  
  
  
  scoped_ptr<content::NavigationEntry> ToNavigationEntry(
      int page_id,
      content::BrowserContext* browser_context) const;

  
  
  
  sync_pb::TabNavigation ToSyncData() const;

  
  
  int index() const { return index_; }
  void set_index(int index) { index_ = index; }

  
  int unique_id() const { return unique_id_; }
  const GURL& virtual_url() const { return virtual_url_; }
  const base::string16& title() const { return title_; }
  const content::PageState& page_state() const { return page_state_; }
  const base::string16& search_terms() const { return search_terms_; }
  const GURL& favicon_url() const { return favicon_url_; }
  int http_status_code() const { return http_status_code_; }
  const content::Referrer& referrer() const { return referrer_; }
  content::PageTransition transition_type() const {
    return transition_type_;
  }
  bool has_post_data() const { return has_post_data_; }
  int64 post_id() const { return post_id_; }
  const GURL& original_request_url() const { return original_request_url_; }
  bool is_overriding_user_agent() const { return is_overriding_user_agent_; }
  base::Time timestamp() const { return timestamp_; }

  BlockedState blocked_state() { return blocked_state_; }
  void set_blocked_state(BlockedState blocked_state) {
    blocked_state_ = blocked_state;
  }
  std::set<std::string> content_pack_categories() {
    return content_pack_categories_;
  }
  void set_content_pack_categories(
      const std::set<std::string>& content_pack_categories) {
    content_pack_categories_ = content_pack_categories;
  }

  
  
  
  static std::vector<content::NavigationEntry*> ToNavigationEntries(
      const std::vector<SerializedNavigationEntry>& navigations,
      content::BrowserContext* browser_context);

 private:
  friend class SerializedNavigationEntryTestHelper;

  
  int index_;

  
  int unique_id_;
  content::Referrer referrer_;
  GURL virtual_url_;
  base::string16 title_;
  content::PageState page_state_;
  content::PageTransition transition_type_;
  bool has_post_data_;
  int64 post_id_;
  GURL original_request_url_;
  bool is_overriding_user_agent_;
  base::Time timestamp_;
  base::string16 search_terms_;
  GURL favicon_url_;
  int http_status_code_;

  
  BlockedState blocked_state_;
  std::set<std::string> content_pack_categories_;
};

}  

#endif  
