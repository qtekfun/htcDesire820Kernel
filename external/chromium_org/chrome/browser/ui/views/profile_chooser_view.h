// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_PROFILE_CHOOSER_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_PROFILE_CHOOSER_VIEW_H_

#include <map>
#include <vector>

#include "chrome/browser/profiles/avatar_menu.h"
#include "chrome/browser/profiles/avatar_menu_observer.h"
#include "google_apis/gaia/oauth2_token_service.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/menu_button_listener.h"
#include "ui/views/controls/link_listener.h"
#include "ui/views/controls/styled_label_listener.h"
#include "ui/views/controls/textfield/textfield_controller.h"

class EditableProfilePhoto;
class EditableProfileName;

namespace gfx {
class Image;
}

namespace views {
class GridLayout;
class Link;
class TextButton;
class LabelButton;
}

class Browser;

class ProfileChooserView : public views::BubbleDelegateView,
                           public views::ButtonListener,
                           public views::LinkListener,
                           public views::MenuButtonListener,
                           public views::TextfieldController,
                           public AvatarMenuObserver,
                           public OAuth2TokenService::Observer {
 public:
  
  
  
  
  
  static void ShowBubble(views::View* anchor_view,
                         views::BubbleBorder::Arrow arrow,
                         views::BubbleBorder::BubbleAlignment border_alignment,
                         const gfx::Rect& anchor_rect,
                         Browser* browser);
  static bool IsShowing();
  static void Hide();

  
  
  
  static void clear_close_on_deactivate_for_testing() {
    close_on_deactivate_for_testing_ = false;
  }

 private:
  friend class NewAvatarMenuButtonTest;
  FRIEND_TEST_ALL_PREFIXES(NewAvatarMenuButtonTest, SignOut);

  typedef std::vector<size_t> Indexes;
  typedef std::map<views::Button*, int> ButtonIndexes;
  typedef std::map<views::View*, std::string> AccountButtonIndexes;

  
  enum BubbleViewMode {
    PROFILE_CHOOSER_VIEW,     
    ACCOUNT_MANAGEMENT_VIEW,  
    GAIA_SIGNIN_VIEW,         
    GAIA_ADD_ACCOUNT_VIEW     
  };

  ProfileChooserView(views::View* anchor_view,
                     views::BubbleBorder::Arrow arrow,
                     const gfx::Rect& anchor_rect,
                     Browser* browser);
  virtual ~ProfileChooserView();

  
  virtual void Init() OVERRIDE;
  virtual void WindowClosing() OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void LinkClicked(views::Link* sender, int event_flags) OVERRIDE;

  
  virtual void OnMenuButtonClicked(views::View* source,
                                   const gfx::Point& point) OVERRIDE;
  
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;

  
  virtual void OnAvatarMenuChanged(AvatarMenu* avatar_menu) OVERRIDE;

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;
  virtual void OnRefreshTokenRevoked(const std::string& account_id) OVERRIDE;

  static ProfileChooserView* profile_bubble_;
  static bool close_on_deactivate_for_testing_;

  void ResetView();

  
  void ShowView(BubbleViewMode view_to_display,
                AvatarMenu* avatar_menu);

  
  
  
  views::View* CreateCurrentProfileView(
      const AvatarMenu::Item& avatar_item,
      bool is_guest);
  views::View* CreateGuestProfileView();
  views::View* CreateOtherProfilesView(const Indexes& avatars_to_show);
  views::View* CreateOptionsView(bool is_guest_view);

  
  views::View* CreateCurrentProfileEditableView(
      const AvatarMenu::Item& avatar_item);
  views::View* CreateCurrentProfileAccountsView(
      const AvatarMenu::Item& avatar_item);
  void CreateAccountButton(views::GridLayout* layout,
                           const std::string& account,
                           bool is_primary_account);

  scoped_ptr<AvatarMenu> avatar_menu_;
  Browser* browser_;

  
  ButtonIndexes open_other_profile_indexes_map_;

  
  AccountButtonIndexes current_profile_accounts_map_;

  
  views::Link* manage_accounts_link_;
  views::Link* signout_current_profile_link_;
  views::Link* signin_current_profile_link_;

  
  
  EditableProfilePhoto* current_profile_photo_;
  EditableProfileName* current_profile_name_;

  
  views::TextButton* guest_button_;
  views::TextButton* end_guest_button_;
  views::TextButton* add_user_button_;
  views::TextButton* users_button_;
  views::LabelButton* add_account_button_;

  
  BubbleViewMode view_mode_;

  DISALLOW_COPY_AND_ASSIGN(ProfileChooserView);
};

#endif  
