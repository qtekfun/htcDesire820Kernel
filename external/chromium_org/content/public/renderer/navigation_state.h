// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_NAVIGATION_STATE_H_
#define CONTENT_PUBLIC_RENDERER_NAVIGATION_STATE_H_

#include <string>

#include "content/common/content_export.h"
#include "content/public/common/page_transition_types.h"

namespace content {

class CONTENT_EXPORT NavigationState {
 public:
  virtual ~NavigationState();

  static NavigationState* CreateBrowserInitiated(
      int32 pending_page_id,
      int pending_history_list_offset,
      bool history_list_was_cleared,
      content::PageTransition transition_type) {
    return new NavigationState(transition_type,
                               false,
                               pending_page_id,
                               pending_history_list_offset,
                               history_list_was_cleared);
  }

  static NavigationState* CreateContentInitiated() {
    return new NavigationState(
        content::PAGE_TRANSITION_LINK, true, -1, -1, false);
  }

  
  int32 pending_page_id() const { return pending_page_id_; }

  
  
  int pending_history_list_offset() const {
    return pending_history_list_offset_;
  }

  
  
  bool history_list_was_cleared() const {
    return history_list_was_cleared_;
  }

  
  
  
  
  
  
  
  
  
  
  bool should_replace_current_entry() const {
    return should_replace_current_entry_;
  }
  void set_should_replace_current_entry(bool value) {
    should_replace_current_entry_ = value;
  }

  
  
  content::PageTransition transition_type() const { return transition_type_; }
  void set_transition_type(content::PageTransition type) {
    transition_type_ = type;
  }

  
  
  bool request_committed() const { return request_committed_; }
  void set_request_committed(bool value) { request_committed_ = value; }

  
  bool is_content_initiated() const { return is_content_initiated_; }

  
  void set_was_within_same_page(bool value) { was_within_same_page_ = value; }
  bool was_within_same_page() const { return was_within_same_page_; }

  
  
  
  
  void set_transferred_request_child_id(int value) {
    transferred_request_child_id_ = value;
  }
  int transferred_request_child_id() const {
    return transferred_request_child_id_;
  }
  void set_transferred_request_request_id(int value) {
    transferred_request_request_id_ = value;
  }
  int transferred_request_request_id() const {
    return transferred_request_request_id_;
  }
  void set_allow_download(bool value) {
    allow_download_ = value;
  }
  bool allow_download() const {
    return allow_download_;
  }

  void set_extra_headers(const std::string& extra_headers) {
    extra_headers_ = extra_headers;
  }
  const std::string& extra_headers() { return extra_headers_; }

 private:
  NavigationState(content::PageTransition transition_type,
                  bool is_content_initiated,
                  int32 pending_page_id,
                  int pending_history_list_offset,
                  bool history_list_was_cleared);

  content::PageTransition transition_type_;
  bool request_committed_;
  bool is_content_initiated_;
  int32 pending_page_id_;
  int pending_history_list_offset_;
  bool history_list_was_cleared_;
  bool should_replace_current_entry_;

  bool was_within_same_page_;
  int transferred_request_child_id_;
  int transferred_request_request_id_;
  bool allow_download_;
  std::string extra_headers_;

  DISALLOW_COPY_AND_ASSIGN(NavigationState);
};

}  

#endif  
