// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TRANSLATE_TRANSLATE_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_TRANSLATE_TRANSLATE_BUBBLE_VIEW_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/ui/translate/language_combobox_model.h"
#include "chrome/browser/ui/translate/translate_bubble_model.h"
#include "chrome/common/translate/translate_errors.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/combobox/combobox_listener.h"
#include "ui/views/controls/link_listener.h"

class Browser;
class PrefService;
class TranslateBubbleModel;

namespace views {
class Checkbox;
class GridLayout;
class LabelButton;
class Link;
class View;
}

class TranslateBubbleView : public views::BubbleDelegateView,
                            public views::ButtonListener,
                            public views::ComboboxListener,
                            public views::LinkListener,
                            public content::WebContentsObserver {
 public:
  virtual ~TranslateBubbleView();

  
  static void ShowBubble(views::View* anchor_view,
                         content::WebContents* web_contents,
                         TranslateBubbleModel::ViewState type,
                         TranslateErrors::Type error_type,
                         Browser* browser);

  
  static bool IsShowing();

  
  static TranslateBubbleView* GetCurrentBubble();

  TranslateBubbleModel* model() { return model_.get(); }

  
  virtual void Init() OVERRIDE;
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void WindowClosing() OVERRIDE;

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual void OnSelectedIndexChanged(views::Combobox* combobox) OVERRIDE;
  virtual void OnComboboxTextButtonClicked(views::Combobox* combobox) OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  
  virtual void WebContentsDestroyed(content::WebContents* web_contents)
      OVERRIDE;

  
  TranslateBubbleModel::ViewState GetViewState() const;

 private:
  enum LinkID {
    LINK_ID_ADVANCED,
    LINK_ID_LEARN_MORE,
  };

  enum ButtonID {
    BUTTON_ID_TRANSLATE,
    BUTTON_ID_DONE,
    BUTTON_ID_CANCEL,
    BUTTON_ID_SHOW_ORIGINAL,
    BUTTON_ID_TRY_AGAIN,
    BUTTON_ID_ALWAYS_TRANSLATE,
  };

  enum ComboboxID {
    COMBOBOX_ID_DENIAL,
    COMBOBOX_ID_SOURCE_LANGUAGE,
    COMBOBOX_ID_TARGET_LANGUAGE,
  };

  friend class TranslateBubbleViewTest;
  FRIEND_TEST_ALL_PREFIXES(TranslateBubbleViewTest, TranslateButton);
  FRIEND_TEST_ALL_PREFIXES(TranslateBubbleViewTest, AdvancedLink);
  FRIEND_TEST_ALL_PREFIXES(TranslateBubbleViewTest, ShowOriginalButton);
  FRIEND_TEST_ALL_PREFIXES(TranslateBubbleViewTest, TryAgainButton);
  FRIEND_TEST_ALL_PREFIXES(TranslateBubbleViewTest,
                           AlwaysTranslateCheckboxAndCancelButton);
  FRIEND_TEST_ALL_PREFIXES(TranslateBubbleViewTest,
                           AlwaysTranslateCheckboxAndDoneButton);
  FRIEND_TEST_ALL_PREFIXES(TranslateBubbleViewTest, DoneButton);
  FRIEND_TEST_ALL_PREFIXES(TranslateBubbleViewTest,
                           DoneButtonWithoutTranslating);
  FRIEND_TEST_ALL_PREFIXES(TranslateBubbleViewTest,
                           CancelButtonReturningBeforeTranslate);
  FRIEND_TEST_ALL_PREFIXES(TranslateBubbleViewTest,
                           CancelButtonReturningAfterTranslate);
  FRIEND_TEST_ALL_PREFIXES(TranslateBubbleViewTest, CancelButtonReturningError);

  TranslateBubbleView(views::View* anchor_view,
                      scoped_ptr<TranslateBubbleModel> model,
                      TranslateErrors::Type error_type,
                      Browser* browser,
                      content::WebContents* web_contents);

  
  views::View* GetCurrentView();

  
  void HandleButtonPressed(ButtonID sender_id);

  
  void HandleLinkClicked(LinkID sender_id);

  
  void HandleComboboxSelectedIndexChanged(ComboboxID sender_id);

  
  void UpdateChildVisibilities();

  
  
  views::View* CreateViewBeforeTranslate();

  
  
  views::View* CreateViewTranslating();

  
  
  views::View* CreateViewAfterTranslate();

  
  views::View* CreateViewError();

  
  views::View* CreateViewAdvanced();

  
  void SwitchView(TranslateBubbleModel::ViewState view_state);

  
  void SwitchToErrorView(TranslateErrors::Type error_type);

  
  void UpdateAdvancedView();

  static TranslateBubbleView* translate_bubble_view_;

  views::View* before_translate_view_;
  views::View* translating_view_;
  views::View* after_translate_view_;
  views::View* error_view_;
  views::View* advanced_view_;

  scoped_ptr<LanguageComboboxModel> source_language_combobox_model_;
  scoped_ptr<LanguageComboboxModel> target_language_combobox_model_;

  views::Combobox* denial_combobox_;
  views::Combobox* source_language_combobox_;
  views::Combobox* target_language_combobox_;

  views::Checkbox* always_translate_checkbox_;

  views::LabelButton* advanced_cancel_button_;
  views::LabelButton* advanced_done_button_;

  scoped_ptr<TranslateBubbleModel> model_;

  TranslateErrors::Type error_type_;

  
  const bool is_in_incognito_window_;

  
  Browser* browser_;

  
  bool translate_executed_;

  
  bool denial_button_clicked_;

  DISALLOW_COPY_AND_ASSIGN(TranslateBubbleView);
};

#endif  
