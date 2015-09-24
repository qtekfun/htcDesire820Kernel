// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_SIMPLE_MENU_MODEL_H_
#define UI_BASE_MODELS_SIMPLE_MENU_MODEL_H_

#include <vector>

#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "ui/base/models/menu_model.h"

namespace gfx {
class Image;
}

namespace ui {

class ButtonMenuItemModel;

class UI_EXPORT SimpleMenuModel : public MenuModel {
 public:
  class UI_EXPORT Delegate {
   public:
    
    virtual bool IsCommandIdChecked(int command_id) const = 0;
    virtual bool IsCommandIdEnabled(int command_id) const = 0;
    virtual bool IsCommandIdVisible(int command_id) const;

    
    
    virtual bool GetAcceleratorForCommandId(
        int command_id,
        ui::Accelerator* accelerator) = 0;

    
    
    virtual bool IsItemForCommandIdDynamic(int command_id) const;
    virtual base::string16 GetLabelForCommandId(int command_id) const;
    virtual base::string16 GetSublabelForCommandId(int command_id) const;
    virtual base::string16 GetMinorTextForCommandId(int command_id) const;
    
    
    virtual bool GetIconForCommandId(int command_id,
                                     gfx::Image* icon) const;

    
    
    virtual void CommandIdHighlighted(int command_id);

    
    
    
    virtual void ExecuteCommand(int command_id, int event_flags) = 0;

    
    virtual void MenuWillShow(SimpleMenuModel* source);

    
    virtual void MenuClosed(SimpleMenuModel* source);

   protected:
    virtual ~Delegate() {}
  };

  
  
  explicit SimpleMenuModel(Delegate* delegate);
  virtual ~SimpleMenuModel();

  
  void AddItem(int command_id, const base::string16& label);
  void AddItemWithStringId(int command_id, int string_id);
  void AddCheckItem(int command_id, const base::string16& label);
  void AddCheckItemWithStringId(int command_id, int string_id);
  void AddRadioItem(int command_id, const base::string16& label, int group_id);
  void AddRadioItemWithStringId(int command_id, int string_id, int group_id);

  
  
  
  
  
  void AddSeparator(MenuSeparatorType separator_type);

  
  
  void RemoveTrailingSeparators();

  
  
  void AddButtonItem(int command_id, ButtonMenuItemModel* model);
  void AddSubMenu(int command_id,
                  const base::string16& label,
                  MenuModel* model);
  void AddSubMenuWithStringId(int command_id, int string_id, MenuModel* model);

  
  void InsertItemAt(int index, int command_id, const base::string16& label);
  void InsertItemWithStringIdAt(int index, int command_id, int string_id);
  void InsertSeparatorAt(int index, MenuSeparatorType separator_type);
  void InsertCheckItemAt(int index,
                         int command_id,
                         const base::string16& label);
  void InsertCheckItemWithStringIdAt(int index, int command_id, int string_id);
  void InsertRadioItemAt(int index,
                         int command_id,
                         const base::string16& label,
                         int group_id);
  void InsertRadioItemWithStringIdAt(
      int index, int command_id, int string_id, int group_id);
  void InsertSubMenuAt(int index,
                       int command_id,
                       const base::string16& label,
                       MenuModel* model);
  void InsertSubMenuWithStringIdAt(
      int index, int command_id, int string_id, MenuModel* model);

  
  void RemoveItemAt(int index);

  
  void SetIcon(int index, const gfx::Image& icon);

  
  void SetSublabel(int index, const base::string16& sublabel);

  
  void SetMinorText(int index, const base::string16& minor_text);

  
  void Clear();

  
  
  int GetIndexOfCommandId(int command_id);

  
  virtual bool HasIcons() const OVERRIDE;
  virtual int GetItemCount() const OVERRIDE;
  virtual ItemType GetTypeAt(int index) const OVERRIDE;
  virtual ui::MenuSeparatorType GetSeparatorTypeAt(int index) const OVERRIDE;
  virtual int GetCommandIdAt(int index) const OVERRIDE;
  virtual base::string16 GetLabelAt(int index) const OVERRIDE;
  virtual base::string16 GetSublabelAt(int index) const OVERRIDE;
  virtual base::string16 GetMinorTextAt(int index) const OVERRIDE;
  virtual bool IsItemDynamicAt(int index) const OVERRIDE;
  virtual bool GetAcceleratorAt(int index,
                                ui::Accelerator* accelerator) const OVERRIDE;
  virtual bool IsItemCheckedAt(int index) const OVERRIDE;
  virtual int GetGroupIdAt(int index) const OVERRIDE;
  virtual bool GetIconAt(int index, gfx::Image* icon) OVERRIDE;
  virtual ui::ButtonMenuItemModel* GetButtonMenuItemAt(
      int index) const OVERRIDE;
  virtual bool IsEnabledAt(int index) const OVERRIDE;
  virtual bool IsVisibleAt(int index) const OVERRIDE;
  virtual void HighlightChangedTo(int index) OVERRIDE;
  virtual void ActivatedAt(int index) OVERRIDE;
  virtual void ActivatedAt(int index, int event_flags) OVERRIDE;
  virtual MenuModel* GetSubmenuModelAt(int index) const OVERRIDE;
  virtual void MenuWillShow() OVERRIDE;
  virtual void MenuClosed() OVERRIDE;
  virtual void SetMenuModelDelegate(
      ui::MenuModelDelegate* menu_model_delegate) OVERRIDE;
  virtual MenuModelDelegate* GetMenuModelDelegate() const OVERRIDE;

 protected:
  void set_delegate(Delegate* delegate) { delegate_ = delegate; }
  Delegate* delegate() { return delegate_; }

  
  
  virtual void MenuItemsChanged();

 private:
  struct Item;

  typedef std::vector<Item> ItemVector;

  
  int ValidateItemIndex(int index) const;

  
  void AppendItem(const Item& item);
  void InsertItemAtIndex(const Item& item, int index);
  void ValidateItem(const Item& item);

  
  void OnMenuClosed();

  ItemVector items_;

  Delegate* delegate_;

  MenuModelDelegate* menu_model_delegate_;

  base::WeakPtrFactory<SimpleMenuModel> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(SimpleMenuModel);
};

}  

#endif  
