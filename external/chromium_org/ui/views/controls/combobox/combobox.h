// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_COMBOBOX_COMBOBOX_H_
#define UI_VIEWS_CONTROLS_COMBOBOX_COMBOBOX_H_

#include <string>

#include "base/time/time.h"
#include "ui/base/models/combobox_model_observer.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/menu/menu_delegate.h"
#include "ui/views/controls/prefix_delegate.h"

namespace gfx {
class Font;
class SlideAnimation;
}

namespace ui {
class ComboboxModel;
}

namespace views {

class ComboboxListener;
class ComboboxMenuRunner;
class CustomButton;
class FocusableBorder;
class MenuRunner;
class MenuRunnerHandler;
class Painter;
class PrefixSelector;

class VIEWS_EXPORT Combobox : public MenuDelegate,
                              public PrefixDelegate,
                              public ui::ComboboxModelObserver,
                              public ButtonListener {
 public:
  
  enum Style {
    STYLE_SHOW_DROP_DOWN_ON_CLICK,
    STYLE_NOTIFY_ON_CLICK,
  };

  
  static const char kViewClassName[];

  
  explicit Combobox(ui::ComboboxModel* model);
  virtual ~Combobox();

  static const gfx::Font& GetFont();

  
  void set_listener(ComboboxListener* listener) { listener_ = listener; }

  void SetStyle(Style style);

  
  void ModelChanged();

  
  int selected_index() const { return selected_index_; }
  void SetSelectedIndex(int index);

  
  
  bool SelectValue(const base::string16& value);

  ui::ComboboxModel* model() const { return model_; }

  
  void SetAccessibleName(const string16& name);

  
  
  
  void SetInvalid(bool invalid);
  bool invalid() const { return invalid_; }

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual bool SkipDefaultKeyEventProcessing(const ui::KeyEvent& e) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& e) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& e) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual ui::TextInputClient* GetTextInputClient() OVERRIDE;
  virtual void Layout() OVERRIDE;

  
  virtual bool IsItemChecked(int id) const OVERRIDE;
  virtual bool IsCommandEnabled(int id) const OVERRIDE;
  virtual void ExecuteCommand(int id) OVERRIDE;
  virtual bool GetAccelerator(int id, ui::Accelerator* accelerator) OVERRIDE;

  
  virtual int GetRowCount() OVERRIDE;
  virtual int GetSelectedRow() OVERRIDE;
  virtual void SetSelectedRow(int row) OVERRIDE;
  virtual string16 GetTextForRow(int row) OVERRIDE;

  
  virtual void OnModelChanged() OVERRIDE;

  
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(ComboboxTest, Click);
  FRIEND_TEST_ALL_PREFIXES(ComboboxTest, NotifyOnClickWithMouse);

  
  void UpdateFromModel();

  
  void UpdateBorder();

  
  void AdjustBoundsForRTLUI(gfx::Rect* rect) const;

  
  void PaintText(gfx::Canvas* canvas);

  
  void PaintButtons(gfx::Canvas* canvas);

  
  void ShowDropDownMenu(ui::MenuSourceType source_type);

  
  void OnSelectionChanged();

  
  int MenuCommandToIndex(int menu_command_id) const;

  int GetDisclosureArrowLeftPadding() const;
  int GetDisclosureArrowRightPadding() const;

  
  void HandleClickEvent();

  
  ui::ComboboxModel* model_;

  
  Style style_;

  
  ComboboxListener* listener_;

  
  int selected_index_;

  
  bool invalid_;

  
  string16 accessible_name_;

  
  scoped_ptr<PrefixSelector> selector_;

  
  const gfx::ImageSkia* disclosure_arrow_;

  
  scoped_ptr<MenuRunner> dropdown_list_menu_runner_;

  
  bool dropdown_open_;

  
  
  
  
  
  
  base::Time closed_time_;

  
  gfx::Size content_size_;

  
  
  
  scoped_ptr<Painter> body_button_painters_[2][Button::STATE_COUNT];
  std::vector<const gfx::ImageSkia*>
      menu_button_images_[2][Button::STATE_COUNT];

  
  
  
  
  
  
  CustomButton* text_button_;
  CustomButton* arrow_button_;

  DISALLOW_COPY_AND_ASSIGN(Combobox);
};

}  

#endif  
