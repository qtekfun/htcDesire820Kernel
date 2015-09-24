// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_DELEGATE_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_DELEGATE_H_

#include <set>
#include <string>

#include "base/logging.h"
#include "base/strings/string16.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/views/controls/menu/menu_item_view.h"

using ui::OSExchangeData;

namespace gfx {
class Font;
}

namespace ui {
class Accelerator;
}

namespace views {

class MenuButton;


class VIEWS_EXPORT MenuDelegate {
 public:
  
  
  enum DropPosition {
    DROP_UNKNOWN = -1,

    
    DROP_NONE,

    
    DROP_BEFORE,

    
    DROP_AFTER,

    
    DROP_ON
  };

  virtual ~MenuDelegate();

  
  
  virtual bool IsItemChecked(int id) const;

  
  
  virtual string16 GetLabel(int id) const;

  
  virtual const gfx::Font* GetLabelFont(int id) const;

  
  
  
  virtual bool GetForegroundColor(int command_id,
                                  bool is_hovered,
                                  SkColor* override_color) const;

  
  
  
  virtual bool GetBackgroundColor(int command_id,
                                  bool is_hovered,
                                  SkColor* override_color) const;

  
  
  virtual string16 GetTooltipText(int id, const gfx::Point& screen_loc) const;

  
  
  virtual bool GetAccelerator(int id, ui::Accelerator* accelerator);

  
  
  
  
  
  
  
  
  virtual bool ShowContextMenu(MenuItemView* source,
                               int id,
                               const gfx::Point& p,
                               ui::MenuSourceType source_type);

  
  virtual bool SupportsCommand(int id) const;
  virtual bool IsCommandEnabled(int id) const;
  virtual bool GetContextualLabel(int id, string16* out) const;
  virtual void ExecuteCommand(int id) {
  }

  
  
  
  
  
  virtual bool ShouldCloseAllMenusOnExecute(int id);

  
  
  
  
  virtual void ExecuteCommand(int id, int mouse_event_flags);

  
  
  virtual bool ShouldExecuteCommandWithoutClosingMenu(int id,
                                                      const ui::Event& e);

  
  
  virtual bool IsTriggerableEvent(MenuItemView* view, const ui::Event& e);

  
  
  
  
  
  
  
  
  
  
  
  

  
  virtual bool CanDrop(MenuItemView* menu, const OSExchangeData& data);

  
  virtual bool GetDropFormats(
      MenuItemView* menu,
      int* formats,
      std::set<OSExchangeData::CustomFormat>* custom_formats);

  
  virtual bool AreDropTypesRequired(MenuItemView* menu);

  
  
  
  
  
  
  virtual int GetDropOperation(MenuItemView* item,
                               const ui::DropTargetEvent& event,
                               DropPosition* position);

  
  
  
  
  
  virtual int OnPerformDrop(MenuItemView* menu,
                            DropPosition position,
                            const ui::DropTargetEvent& event);

  
  
  virtual bool CanDrag(MenuItemView* menu);

  
  
  virtual void WriteDragData(MenuItemView* sender, OSExchangeData* data);

  
  
  virtual int GetDragOperations(MenuItemView* sender);

  
  
  virtual void DropMenuClosed(MenuItemView* menu) {
  }

  
  virtual void SelectionChanged(MenuItemView* menu) {
  }

  
  
  
  
  
  virtual MenuItemView* GetSiblingMenu(MenuItemView* menu,
                                       const gfx::Point& screen_point,
                                       MenuItemView::AnchorPosition* anchor,
                                       bool* has_mnemonics,
                                       MenuButton** button);

  
  virtual int GetMaxWidthForMenu(MenuItemView* menu);

  
  virtual void WillShowMenu(MenuItemView* menu);

  
  virtual void WillHideMenu(MenuItemView* menu);

  
  
  
  
  virtual void GetHorizontalIconMargins(int command_id,
                                        int icon_size,
                                        int* left_margin,
                                        int* right_margin) const;
  
  
  virtual bool ShouldReserveSpaceForSubmenuIndicator() const;
};

}  

#endif  
