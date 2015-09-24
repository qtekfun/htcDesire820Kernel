// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

namespace gfx {
class ImageSkia;
class Point;
}

namespace ui {
class Accelerator;
}

namespace views {

class VIEWS_EXPORT Menu {
 public:
  
  
  
  
  
  
  
  
  class VIEWS_EXPORT Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual bool IsItemChecked(int id) const;

    
    
    virtual bool IsItemDefault(int id) const;

    
    virtual string16 GetLabel(int id) const;

    
    
    
    virtual bool GetAcceleratorInfo(int id, ui::Accelerator* accel);

    
    virtual const gfx::ImageSkia& GetIcon(int id) const;

    
    virtual int GetItemCount() const;

    
    virtual bool IsItemSeparator(int id) const;

    
    
    
    
    
    
    
    virtual void ShowContextMenu(Menu* source,
                                 int id,
                                 const gfx::Point& p,
                                 bool is_mouse_gesture) {
    }

    
    virtual bool HasIcon(int id) const;

    
    virtual void MenuWillShow() {
    }

    
    
    
    
    
    
    
    
    virtual bool IsRightToLeftUILayout() const;

    
    virtual bool SupportsCommand(int id) const;
    virtual bool IsCommandEnabled(int id) const;
    virtual bool GetContextualLabel(int id, string16* out) const;
    virtual void ExecuteCommand(int id) {
    }

   protected:
    
    const gfx::ImageSkia& GetEmptyIcon() const;
  };

  
  enum AnchorPoint {
    TOPLEFT,
    TOPRIGHT
  };

  
  enum MenuItemType {
    NORMAL,
    CHECKBOX,
    RADIO,
    SEPARATOR
  };

  
  
  
  
  
  
  
  
  Menu(Delegate* delegate, AnchorPoint anchor);
  Menu();
  virtual ~Menu();

  static Menu* Create(Delegate* delegate,
                      AnchorPoint anchor,
                      gfx::NativeView parent);

  
  
  static Menu* GetSystemMenu(gfx::NativeWindow parent);

  void set_delegate(Delegate* delegate) { delegate_ = delegate; }
  Delegate* delegate() const { return delegate_; }

  AnchorPoint anchor() const { return anchor_; }

  
  
  
  
  
  
  
  
  void AppendMenuItem(int item_id,
                      const string16& label,
                      MenuItemType type);
  void AddMenuItem(int index,
                   int item_id,
                   const string16& label,
                   MenuItemType type);

  
  
  Menu* AppendSubMenu(int item_id,
                      const string16& label);
  Menu* AddSubMenu(int index, int item_id, const string16& label);

  
  
  
  
  Menu* AppendSubMenuWithIcon(int item_id,
                              const string16& label,
                              const gfx::ImageSkia& icon);
  virtual Menu* AddSubMenuWithIcon(int index,
                                   int item_id,
                                   const string16& label,
                                   const gfx::ImageSkia& icon) = 0;

  
  
  void AppendMenuItemWithLabel(int item_id, const string16& label);
  void AddMenuItemWithLabel(int index, int item_id, const string16& label);

  
  
  void AppendDelegateMenuItem(int item_id);
  void AddDelegateMenuItem(int index, int item_id);

  
  void AppendSeparator();
  virtual void AddSeparator(int index) = 0;

  
  
  
  void AppendMenuItemWithIcon(int item_id,
                              const string16& label,
                              const gfx::ImageSkia& icon);
  virtual void AddMenuItemWithIcon(int index,
                                   int item_id,
                                   const string16& label,
                                   const gfx::ImageSkia& icon);

  
  virtual void EnableMenuItemByID(int item_id, bool enabled) = 0;
  virtual void EnableMenuItemAt(int index, bool enabled) = 0;

  
  virtual void SetMenuLabel(int item_id, const string16& label) = 0;

  
  
  
  virtual bool SetIcon(const gfx::ImageSkia& icon, int item_id) = 0;

  
  
  
  virtual void RunMenuAt(int x, int y) = 0;

  
  virtual void Cancel() = 0;

  
  virtual int ItemCount() = 0;

#if defined(OS_WIN)
  
  virtual HMENU GetMenuHandle() const = 0;
#endif  

 protected:
  explicit Menu(Menu* parent);

  virtual void AddMenuItemInternal(int index,
                                   int item_id,
                                   const string16& label,
                                   const gfx::ImageSkia& icon,
                                   MenuItemType type) = 0;

 private:
  
  Delegate* delegate_;

  
  AnchorPoint anchor_;

  DISALLOW_COPY_AND_ASSIGN(Menu);
};

}  

#endif  
