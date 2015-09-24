// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_SEARCH_BOX_VIEW_H_
#define UI_APP_LIST_VIEWS_SEARCH_BOX_VIEW_H_

#include <string>

#include "ui/app_list/search_box_model_observer.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/button/menu_button_listener.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/view.h"

namespace views {
class ImageView;
class MenuButton;
class Textfield;
}  

namespace app_list {

class AppListMenuViews;
class AppListModel;
class AppListViewDelegate;
class SearchBoxModel;
class SearchBoxViewDelegate;

class SearchBoxView : public views::View,
                      public views::TextfieldController,
                      public views::ButtonListener,
                      public views::MenuButtonListener,
                      public SearchBoxModelObserver {
 public:
  SearchBoxView(SearchBoxViewDelegate* delegate,
                AppListViewDelegate* view_delegate);
  virtual ~SearchBoxView();

  void ModelChanged();
  bool HasSearch() const;
  void ClearSearch();
  void InvalidateMenu();

  views::Textfield* search_box() { return search_box_; }

  void set_contents_view(View* contents_view) {
    contents_view_ = contents_view;
  }

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;

 private:
  
  void UpdateModel();

  
  void NotifyQueryChanged();

  
  virtual void ContentsChanged(views::Textfield* sender,
                               const base::string16& new_contents) OVERRIDE;
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void OnMenuButtonClicked(View* source,
                                   const gfx::Point& point) OVERRIDE;

  
  virtual void IconChanged() OVERRIDE;
  virtual void SpeechRecognitionButtonPropChanged() OVERRIDE;
  virtual void HintTextChanged() OVERRIDE;
  virtual void SelectionModelChanged() OVERRIDE;
  virtual void TextChanged() OVERRIDE;

  SearchBoxViewDelegate* delegate_;  
  AppListViewDelegate* view_delegate_;  
  AppListModel* model_;  

  scoped_ptr<AppListMenuViews> menu_;

  views::ImageView* icon_view_;  
  views::ImageButton* speech_button_;  
  views::MenuButton* menu_button_;  
  views::Textfield* search_box_;  
  views::View* contents_view_;  

  DISALLOW_COPY_AND_ASSIGN(SearchBoxView);
};

}  

#endif  
