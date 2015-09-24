// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_ITEM_VIEW_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_ITEM_VIEW_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/strings/string16.h"
#include "build/build_config.h"
#include "ui/base/models/menu_separator_types.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/controls/menu/menu_config.h"
#include "ui/views/view.h"

#if defined(OS_WIN)
#include <windows.h>

#include "ui/native_theme/native_theme.h"
#endif

namespace gfx {
class Font;
}

namespace views {

namespace internal {
class MenuRunnerImpl;
}

class MenuController;
class MenuDelegate;
class SubmenuView;



class VIEWS_EXPORT MenuItemView : public View {
 public:
  friend class MenuController;

  
  static const char kViewClassName[];

  
  static const int kMenuItemViewID;

  
  static const int kEmptyMenuItemViewID;

  
  
  enum Type {
    NORMAL,
    SUBMENU,
    CHECKBOX,
    RADIO,
    SEPARATOR,
    EMPTY
  };

  
  
  
  
  
  enum AnchorPosition {
    TOPLEFT,
    TOPRIGHT,
    BOTTOMCENTER,
    BUBBLE_LEFT,
    BUBBLE_RIGHT,
    BUBBLE_ABOVE,
    BUBBLE_BELOW
  };

  
  
  
  
  enum MenuPosition {
    POSITION_BEST_FIT,
    POSITION_ABOVE_BOUNDS,
    POSITION_BELOW_BOUNDS
  };

  
  struct MenuItemDimensions {
    MenuItemDimensions()
        : standard_width(0),
          children_width(0),
          minor_text_width(0),
          height(0) {}

    
    int standard_width;
    
    int children_width;
    
    int minor_text_width;
    
    int height;
  };

  
  
  explicit MenuItemView(MenuDelegate* delegate);

  
  virtual bool GetTooltipText(const gfx::Point& p,
                              string16* tooltip) const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  
  static int pref_menu_height() { return pref_menu_height_; }

  
  static int label_start() { return label_start_; }

  
  static bool IsBubble(MenuItemView::AnchorPosition anchor);

  
  
  static string16 GetAccessibleNameForMenuItem(
      const string16& item_text, const string16& accelerator_text);

  
  void Cancel();

  
  
  MenuItemView* AddMenuItemAt(int index,
                              int item_id,
                              const string16& label,
                              const string16& sublabel,
                              const string16& minor_text,
                              const gfx::ImageSkia& icon,
                              Type type,
                              ui::MenuSeparatorType separator_style);

  
  
  void RemoveMenuItemAt(int index);

  
  
  
  
  
  
  
  
  MenuItemView* AppendMenuItem(int item_id,
                               const string16& label,
                               Type type);

  
  
  MenuItemView* AppendSubMenu(int item_id,
                              const string16& label);

  
  
  MenuItemView* AppendSubMenuWithIcon(int item_id,
                                      const string16& label,
                                      const gfx::ImageSkia& icon);

  
  
  MenuItemView* AppendMenuItemWithLabel(int item_id,
                                        const string16& label);

  
  
  MenuItemView* AppendDelegateMenuItem(int item_id);

  
  void AppendSeparator();

  
  
  
  MenuItemView* AppendMenuItemWithIcon(int item_id,
                                       const string16& label,
                                       const gfx::ImageSkia& icon);

  
  MenuItemView* AppendMenuItemImpl(int item_id,
                                   const string16& label,
                                   const string16& sublabel,
                                   const string16& minor_text,
                                   const gfx::ImageSkia& icon,
                                   Type type,
                                   ui::MenuSeparatorType separator_style);

  
  
  virtual SubmenuView* CreateSubmenu();

  
  virtual bool HasSubmenu() const;

  
  virtual SubmenuView* GetSubmenu() const;

  
  MenuItemView* GetParentMenuItem() { return parent_menu_item_; }
  const MenuItemView* GetParentMenuItem() const { return parent_menu_item_; }

  
  void SetTitle(const string16& title);
  const string16& title() const { return title_; }

  
  void SetSubtitle(const string16& subtitle);

  
  void SetMinorText(const string16& minor_text);

  
  const Type& GetType() const { return type_; }

  
  
  void SetSelected(bool selected);

  
  bool IsSelected() const { return selected_; }

  
  void SetTooltip(const string16& tooltip, int item_id);

  
  void SetIcon(const gfx::ImageSkia& icon, int item_id);

  
  void SetIcon(const gfx::ImageSkia& icon);

  
  
  void SetIconView(View* icon_view);
  View* icon_view() { return icon_view_; }

  
  void SetCommand(int command) { command_ = command; }

  
  int GetCommand() const { return command_; }

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  const MenuItemDimensions& GetDimensions();

  
  MenuController* GetMenuController();
  const MenuController* GetMenuController() const;

  
  MenuDelegate* GetDelegate();
  const MenuDelegate* GetDelegate() const;
  void set_delegate(MenuDelegate* delegate) { delegate_ = delegate; }

  
  MenuItemView* GetRootMenuItem();
  const MenuItemView* GetRootMenuItem() const;

  
  
  char16 GetMnemonic();

  
  
  void set_has_icons(bool has_icons) {
    has_icons_ = has_icons;
  }
  bool has_icons() const { return has_icons_; }

  
  MenuItemView* GetMenuItemByID(int id);

  
  
  void ChildrenChanged();

  
  virtual void Layout() OVERRIDE;

  
  
  bool has_mnemonics() const { return has_mnemonics_; }

  
  
  void SetMargins(int top_margin, int bottom_margin);

  
  void set_use_right_margin(bool use_right_margin) {
    use_right_margin_ = use_right_margin;
  }

  
  const MenuConfig& GetMenuConfig() const;

 protected:
  
  MenuItemView(MenuItemView* parent, int command, Type type);

  
  virtual ~MenuItemView();

  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;

  virtual const char* GetClassName() const OVERRIDE;

  
  virtual gfx::Size GetChildPreferredSize();

  
  int GetTopMargin();
  int GetBottomMargin();

 private:
  friend class internal::MenuRunnerImpl;  

  enum PaintButtonMode { PB_NORMAL, PB_FOR_DRAG };

  
  
  
  void UpdateMenuPartSizes();

  
  void Init(MenuItemView* parent,
            int command,
            MenuItemView::Type type,
            MenuDelegate* delegate);

  
  
  void PrepareForRun(bool is_first_menu,
                     bool has_mnemonics,
                     bool show_mnemonics);

  
  int GetDrawStringFlags();

  
  const gfx::Font& GetFont();

  
  
  
  void AddEmptyMenus();

  
  void RemoveEmptyMenus();

  
  
  void AdjustBoundsForRTLUI(gfx::Rect* rect) const;

  
  
  void PaintButton(gfx::Canvas* canvas, PaintButtonMode mode);

  
  void PaintMinorText(gfx::Canvas* canvas, bool render_selection);

  
  
  void DestroyAllMenuHosts();

  
  
  string16 GetMinorText();

  
  MenuItemDimensions CalculateDimensions();

  
  int GetLabelStartForThisItem();

  
  
  MenuPosition actual_menu_position() const { return actual_menu_position_; }
  void set_actual_menu_position(MenuPosition actual_menu_position) {
    actual_menu_position_ = actual_menu_position;
  }

  void set_controller(MenuController* controller) { controller_ = controller; }

  
  
  bool IsContainer() const;

  
  int NonIconChildViewsCount() const;

  
  int GetMaxIconViewWidth() const;

  
  bool HasChecksOrRadioButtons() const;

  void invalidate_dimensions() { dimensions_.height = 0; }
  bool is_dimensions_valid() const { return dimensions_.height > 0; }

  
  
  
  MenuDelegate* delegate_;

  
  MenuController* controller_;

  
  bool canceled_;

  
  MenuItemView* parent_menu_item_;

  
  
  Type type_;

  
  bool selected_;

  
  int command_;

  
  SubmenuView* submenu_;

  
  string16 title_;

  
  string16 subtitle_;

  
  string16 minor_text_;

  
  bool has_mnemonics_;

  
  
  bool show_mnemonics_;

  
  bool has_icons_;

  
  View* icon_view_;

  
  string16 tooltip_;

  
  static int icon_area_width_;

  
  static int label_start_;

  
  static int item_right_margin_;

  
  static int pref_menu_height_;

  
  
  MenuItemDimensions dimensions_;

  
  std::vector<View*> removed_items_;

  
  int top_margin_;
  int bottom_margin_;

  
  
  int left_icon_margin_;
  int right_icon_margin_;

  
  
  
  MenuPosition requested_menu_position_;
  MenuPosition actual_menu_position_;

  
  
  bool use_right_margin_;

  DISALLOW_COPY_AND_ASSIGN(MenuItemView);
};

}  

#endif  
