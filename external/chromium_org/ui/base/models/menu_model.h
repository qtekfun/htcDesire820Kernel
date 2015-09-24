// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_MENU_MODEL_H_
#define UI_BASE_MODELS_MENU_MODEL_H_

#include "base/strings/string16.h"
#include "ui/base/models/menu_model_delegate.h"
#include "ui/base/models/menu_separator_types.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Font;
class Image;
}

namespace ui {

class Accelerator;
class ButtonMenuItemModel;

class UI_EXPORT MenuModel {
 public:
  
  enum ItemType {
    TYPE_COMMAND,
    TYPE_CHECK,
    TYPE_RADIO,
    TYPE_SEPARATOR,
    TYPE_BUTTON_ITEM,
    TYPE_SUBMENU
  };

  virtual ~MenuModel() {}

  
  
  
  virtual bool HasIcons() const = 0;

  
  virtual int GetItemCount() const = 0;

  
  virtual ItemType GetTypeAt(int index) const = 0;

  
  virtual ui::MenuSeparatorType GetSeparatorTypeAt(int index) const = 0;

  
  virtual int GetCommandIdAt(int index) const = 0;

  
  virtual base::string16 GetLabelAt(int index) const = 0;

  
  
  virtual base::string16 GetSublabelAt(int index) const;

  
  
  virtual base::string16 GetMinorTextAt(int index) const;

  
  
  
  
  virtual bool IsItemDynamicAt(int index) const = 0;

  
  
  virtual const gfx::Font* GetLabelFontAt(int index) const;

  
  
  virtual bool GetAcceleratorAt(int index,
                                ui::Accelerator* accelerator) const = 0;

  
  virtual bool IsItemCheckedAt(int index) const = 0;

  
  
  virtual int GetGroupIdAt(int index) const = 0;

  
  
  virtual bool GetIconAt(int index, gfx::Image* icon) = 0;

  
  virtual ButtonMenuItemModel* GetButtonMenuItemAt(int index) const = 0;

  
  virtual bool IsEnabledAt(int index) const = 0;

  
  virtual bool IsVisibleAt(int index) const;

  
  virtual MenuModel* GetSubmenuModelAt(int index) const = 0;

  
  
  virtual void HighlightChangedTo(int index) = 0;

  
  virtual void ActivatedAt(int index) = 0;

  
  
  
  virtual void ActivatedAt(int index, int event_flags);

  
  virtual void MenuWillShow() {}

  
  virtual void MenuClosed() {}

  
  virtual void SetMenuModelDelegate(MenuModelDelegate* delegate) = 0;

  
  virtual MenuModelDelegate* GetMenuModelDelegate() const = 0;

  
  
  
  static bool GetModelAndIndexForCommandId(int command_id,
                                           MenuModel** model,
                                           int* index);
};

}  

#endif  
