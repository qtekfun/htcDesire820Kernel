// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STATUS_ICONS_STATUS_ICON_MENU_MODEL_H_
#define CHROME_BROWSER_STATUS_ICONS_STATUS_ICON_MENU_MODEL_H_

#include <map>

#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "ui/base/models/simple_menu_model.h"

namespace gfx {
class Image;
}

class StatusIconMenuModelTest;

class StatusIconMenuModel
    : public ui::SimpleMenuModel,
      public ui::SimpleMenuModel::Delegate,
      public base::SupportsWeakPtr<StatusIconMenuModel> {
 public:
  class Delegate {
   public:
    
    
    virtual void CommandIdHighlighted(int command_id);

    
    
    
    virtual void ExecuteCommand(int command_id, int event_flags) = 0;

   protected:
    virtual ~Delegate() {}
  };

  class Observer {
   public:
    
    virtual void OnMenuStateChanged() {}

   protected:
    virtual ~Observer() {}
  };

  
  explicit StatusIconMenuModel(Delegate* delegate);
  virtual ~StatusIconMenuModel();

  
  void SetCommandIdChecked(int command_id, bool checked);
  void SetCommandIdEnabled(int command_id, bool enabled);
  void SetCommandIdVisible(int command_id, bool visible);

  
  void SetAcceleratorForCommandId(
      int command_id, const ui::Accelerator* accelerator);

  
  
  
  
  void ChangeLabelForCommandId(int command_id, const base::string16& label);
  void ChangeSublabelForCommandId(
      int command_id, const base::string16& sublabel);
  void ChangeIconForCommandId(int command_id, const gfx::Image& icon);

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool IsCommandIdVisible(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id, ui::Accelerator* accelerator) OVERRIDE;
  virtual bool IsItemForCommandIdDynamic(int command_id) const OVERRIDE;
  virtual base::string16 GetLabelForCommandId(int command_id) const OVERRIDE;
  virtual base::string16 GetSublabelForCommandId(int command_id) const OVERRIDE;
  virtual bool GetIconForCommandId(int command_id, gfx::Image* icon) const
      OVERRIDE;

 protected:
  
  virtual void MenuItemsChanged() OVERRIDE;

  void NotifyMenuStateChanged();

  void set_delegate(Delegate* delegate) { delegate_ = delegate; }
  Delegate* delegate() { return delegate_; }

 private:
  
  virtual void CommandIdHighlighted(int command_id) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  struct ItemState;

  
  typedef std::map<int, ItemState> ItemStateMap;

  ItemStateMap item_states_;

  ObserverList<Observer> observer_list_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(StatusIconMenuModel);
};

#endif  
