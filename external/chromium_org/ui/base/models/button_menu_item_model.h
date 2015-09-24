// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_BUTTON_MENU_ITEM_MODEL_H_
#define UI_BASE_MODELS_BUTTON_MENU_ITEM_MODEL_H_

#include <vector>

#include "base/strings/string16.h"
#include "ui/base/ui_export.h"

namespace ui {

class UI_EXPORT ButtonMenuItemModel {
 public:
  
  enum ButtonType {
    TYPE_SPACE,
    TYPE_BUTTON,
    TYPE_BUTTON_LABEL
  };

  class UI_EXPORT Delegate {
   public:
    
    virtual bool IsItemForCommandIdDynamic(int command_id) const;
    virtual base::string16 GetLabelForCommandId(int command_id) const;

    
    virtual void ExecuteCommand(int command_id, int event_flags) = 0;
    virtual bool IsCommandIdEnabled(int command_id) const;
    virtual bool DoesCommandIdDismissMenu(int command_id) const;

   protected:
    virtual ~Delegate() {}
  };

  ButtonMenuItemModel(int string_id, ButtonMenuItemModel::Delegate* delegate);
  ~ButtonMenuItemModel();

  
  
  void AddGroupItemWithStringId(int command_id, int string_id);

  
  void AddItemWithImage(int command_id, int icon_idr);

  
  
  
  void AddButtonLabel(int command_id, int string_id);

  
  void AddSpace();

  
  int GetItemCount() const;

  
  ButtonType GetTypeAt(int index) const;

  
  int GetCommandIdAt(int index) const;

  
  bool IsItemDynamicAt(int index) const;

  
  base::string16 GetLabelAt(int index) const;

  
  
  bool GetIconAt(int index, int* icon) const;

  
  
  bool PartOfGroup(int index) const;

  
  void ActivatedCommand(int command_id);

  
  bool IsEnabledAt(int index) const;

  
  bool DismissesMenuAt(int index) const;

  
  bool IsCommandIdEnabled(int command_id) const;

  
  bool DoesCommandIdDismissMenu(int command_id) const;

  const base::string16& label() const { return item_label_; }

 private:
  
  base::string16 item_label_;

  struct Item;
  std::vector<Item> items_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(ButtonMenuItemModel);
};

}  

#endif  
