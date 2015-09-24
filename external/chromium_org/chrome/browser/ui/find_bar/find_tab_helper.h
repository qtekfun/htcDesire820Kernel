// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_FIND_BAR_FIND_TAB_HELPER_H_
#define CHROME_BROWSER_UI_FIND_BAR_FIND_TAB_HELPER_H_

#include "chrome/browser/ui/find_bar/find_bar_controller.h"
#include "chrome/browser/ui/find_bar/find_notification_details.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "ui/gfx/range/range.h"

namespace gfx {
class RectF;
}

class FindTabHelper : public content::WebContentsObserver,
                      public content::WebContentsUserData<FindTabHelper> {
 public:
  virtual ~FindTabHelper();

  
  
  
  
  
  
  void StartFinding(base::string16 search_string,
                    bool forward_direction,
                    bool case_sensitive);

  
  void StopFinding(FindBarController::SelectionAction selection_action);

  
  bool find_ui_active() const { return find_ui_active_; }
  void set_find_ui_active(bool find_ui_active) {
      find_ui_active_ = find_ui_active;
  }

  
  void set_find_op_aborted(bool find_op_aborted) {
    find_op_aborted_ = find_op_aborted;
  }

  
  int current_find_request_id() { return current_find_request_id_; }
  void set_current_find_request_id(int current_find_request_id) {
    current_find_request_id_ = current_find_request_id;
  }

  
  
  base::string16 find_text() const { return find_text_; }

  
  base::string16 previous_find_text() const { return previous_find_text_; }

  gfx::Range selected_range() const { return selected_range_; }
  void set_selected_range(const gfx::Range& selected_range) {
    selected_range_ = selected_range;
  }

  
  const FindNotificationDetails& find_result() const {
    return last_search_result_;
  }

#if defined(OS_ANDROID)
  
  
  void ActivateNearestFindResult(float x, float y);

  
  void RequestFindMatchRects(int current_version);
#endif

  void HandleFindReply(int request_id,
                       int number_of_matches,
                       const gfx::Rect& selection_rect,
                       int active_match_ordinal,
                       bool final_update);

 private:
  explicit FindTabHelper(content::WebContents* web_contents);
  friend class content::WebContentsUserData<FindTabHelper>;

  
  
  
  
  static int find_request_id_counter_;

  
  bool find_ui_active_;

  
  
  
  
  
  bool find_op_aborted_;

  
  int current_find_request_id_;

  
  
  
  base::string16 find_text_;

  
  base::string16 previous_find_text_;

  
  gfx::Range selected_range_;

  
  bool last_search_case_sensitive_;

  
  
  
  FindNotificationDetails last_search_result_;

  DISALLOW_COPY_AND_ASSIGN(FindTabHelper);
};

#endif  
