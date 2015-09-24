// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_CANDIDATE_WINDOW_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_CANDIDATE_WINDOW_VIEW_H_

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chromeos/ime/candidate_window.h"
#include "ui/views/view.h"

namespace gfx {
class Font;
}

namespace chromeos {
namespace input_method {

class CandidateView;
class CandidateWindow;
class HidableArea;
class InformationTextArea;

class CandidateWindowView : public views::View {
 public:
  
  class Observer {
   public:
    virtual ~Observer() {}
    
    virtual void OnCandidateCommitted(int index) = 0;

    virtual void OnCandidateWindowOpened() = 0;
    virtual void OnCandidateWindowClosed() = 0;
  };

  explicit CandidateWindowView(views::Widget* parent_frame);
  virtual ~CandidateWindowView();
  void Init();

  
  void AddObserver(Observer* observer) {
    observers_.AddObserver(observer);
  }

  
  void RemoveObserver(Observer* observer) {
    observers_.RemoveObserver(observer);
  }

  
  
  
  void SelectCandidateAt(int index_in_page);

  
  
  
  void OnCandidatePressed(const gfx::Point& point);

  
  void CommitCandidate();

  
  void HideLookupTable();

  
  void HideAuxiliaryText();

  
  void HidePreeditText();

  
  void HideAll();

  
  void ShowLookupTable();

  
  void ShowAuxiliaryText();

  
  void ShowPreeditText();

  
  void UpdateAuxiliaryText(const std::string& utf8_text);

  
  void UpdatePreeditText(const std::string& utf8_text);

  
  
  
  
  static bool ShouldUpdateCandidateViews(
      const CandidateWindow& old_candidate_window,
      const CandidateWindow& new_candidate_window);

  
  
  void UpdateCandidates(const CandidateWindow& candidate_window);

  
  
  
  
  
  
  
  void ResizeAndMoveParentFrame();

  
  
  
  
  
  
  
  
  
  
  
  int GetHorizontalOffset();

  void set_cursor_bounds(const gfx::Rect& cursor_bounds) {
    cursor_bounds_ = cursor_bounds;
  }

  void set_composition_head_bounds(
      const gfx::Rect& composition_head_bounds) {
    composition_head_bounds_ = composition_head_bounds;
  }

  const gfx::Rect& cursor_bounds() const { return cursor_bounds_; }
  const gfx::Rect& composition_head_bounds() const {
    return composition_head_bounds_;
  }

 protected:
  
  virtual void VisibilityChanged(View* starting_from, bool is_visible) OVERRIDE;

  
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(CandidateWindowViewTest,
                           UpdateCandidatesTest_CursorVisibility);
  FRIEND_TEST_ALL_PREFIXES(CandidateWindowViewTest, ShortcutSettingTest);
  FRIEND_TEST_ALL_PREFIXES(CandidateWindowViewTest,
                           DoNotChangeRowHeightWithLabelSwitchTest);

  
  void MaybeInitializeCandidateViews(const CandidateWindow& candidate_window);

  
  InformationTextArea* GetAuxiliaryTextArea();

  
  
  bool IsCandidateWindowOpen() const;

  
  
  void NotifyIfCandidateWindowOpenedOrClosed();

  
  CandidateWindow candidate_window_;

  
  int selected_candidate_index_in_page_;

  
  ObserverList<Observer> observers_;

  
  views::Widget* parent_frame_;

  
  

  
  
  
  InformationTextArea* preedit_area_;
  
  
  
  InformationTextArea* header_area_;
  
  HidableArea* candidate_area_;
  
  std::vector<CandidateView*> candidate_views_;
  
  
  
  InformationTextArea* footer_area_;

  
  gfx::Size previous_shortcut_column_size_;
  gfx::Size previous_candidate_column_size_;
  gfx::Size previous_annotation_column_size_;

  
  gfx::Rect cursor_bounds_;

  
  gfx::Rect composition_head_bounds_;

  
  
  bool should_show_at_composition_head_;

  
  
  bool should_show_upper_side_;

  
  
  bool was_candidate_window_open_;

  
  
  void UpdateParentArea();

  DISALLOW_COPY_AND_ASSIGN(CandidateWindowView);
};

}  
}  

#endif  
