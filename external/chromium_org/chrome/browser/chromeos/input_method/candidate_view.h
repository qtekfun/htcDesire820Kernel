// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_CANDIDATE_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_CANDIDATE_VIEW_H_

#include "base/gtest_prod_util.h"
#include "chromeos/ime/candidate_window.h"
#include "ui/views/controls/label.h"
#include "ui/views/view.h"

namespace chromeos {
namespace input_method {

class CandidateWindowView;

class CandidateView : public views::View {
 public:
  CandidateView(CandidateWindowView* parent_candidate_window,
                int index_in_page,
                CandidateWindow::Orientation orientation);
  virtual ~CandidateView() {}
  
  
  void Init(int shortcut_column_width,
            int candidate_column_width,
            int annotation_column_width,
            int column_height);

  
  void SetCandidateText(const base::string16& text);

  
  void SetShortcutText(const base::string16& text);

  
  void SetAnnotationText(const base::string16& text);

  
  void SetInfolistIcon(bool enable);

  
  
  void Select();

  
  
  void Unselect();

  
  
  void SetRowEnabled(bool enabled);

  
  gfx::Point GetCandidateLabelPosition() const;

 private:
  friend class CandidateWindowViewTest;
  FRIEND_TEST_ALL_PREFIXES(CandidateWindowViewTest, ShortcutSettingTest);
  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  void SelectCandidateAt(const gfx::Point& location);

  
  
  void UpdateLabelBackgroundColors();

  
  int index_in_page_;

  
  CandidateWindow::Orientation orientation_;

  
  CandidateWindowView* parent_candidate_window_;

  
  

  
  views::Label* shortcut_label_;
  
  views::Label* candidate_label_;
  
  views::Label* annotation_label_;

  
  views::View* infolist_icon_;
  bool infolist_icon_enabled_;

  DISALLOW_COPY_AND_ASSIGN(CandidateView);
};

}  
}  

#endif  
