// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_PAGINATION_MODEL_H_
#define UI_APP_LIST_PAGINATION_MODEL_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "ui/app_list/app_list_export.h"
#include "ui/gfx/animation/animation_delegate.h"

namespace gfx {
class SlideAnimation;
}

namespace app_list {

class PaginationModelObserver;

class APP_LIST_EXPORT PaginationModel : public gfx::AnimationDelegate {
 public:
  
  struct Transition {
    Transition(int target_page, double progress)
        : target_page(target_page),
          progress(progress) {
    }

    bool Equals(const Transition& rhs) const {
      return target_page == rhs.target_page && progress == rhs.progress;
    }

    
    
    
    
    int target_page;

    
    
    double progress;
  };

  PaginationModel();
  virtual ~PaginationModel();

  void SetTotalPages(int total_pages);

  
  void SelectPage(int page, bool animate);

  
  void SelectPageRelative(int delta, bool animate);

  void SetTransition(const Transition& transition);
  void SetTransitionDurations(int duration_ms, int overscroll_duration_ms);

  
  
  void StartScroll();

  
  
  
  void UpdateScroll(double delta);

  
  
  void EndScroll(bool cancel);

  
  bool IsRevertingCurrentTransition() const;

  void AddObserver(PaginationModelObserver* observer);
  void RemoveObserver(PaginationModelObserver* observer);

  int total_pages() const { return total_pages_; }
  int selected_page() const { return selected_page_; }
  const Transition& transition() const { return transition_; }

  bool is_valid_page(int page) const {
    return page >= 0 && page < total_pages_;
  }

  bool has_transition() const {
    return transition_.target_page != -1 || transition_.progress != 0;
  }

 private:
  void NotifySelectedPageChanged(int old_selected, int new_selected);
  void NotifyTransitionStarted();
  void NotifyTransitionChanged();

  void clear_transition() {
    SetTransition(Transition(-1, 0));
  }

  
  
  
  
  
  
  int CalculateTargetPage(int delta) const;

  void StartTransitionAnimation(const Transition& transition);
  void ResetTransitionAnimation();

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;

  int total_pages_;
  int selected_page_;

  Transition transition_;

  
  
  
  int pending_selected_page_;

  scoped_ptr<gfx::SlideAnimation> transition_animation_;
  int transition_duration_ms_;  
  int overscroll_transition_duration_ms_;

  int last_overscroll_target_page_;
  base::TimeTicks last_overscroll_animation_start_time_;

  ObserverList<PaginationModelObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(PaginationModel);
};

}  

#endif  
