// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_RUNNER_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_RUNNER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/views/controls/menu/menu_item_view.h"

namespace ui {
class MenuModel;
}

namespace views {

class MenuButton;
class MenuModelAdapter;
class MenuRunnerHandler;
class Widget;

namespace internal {
class DisplayChangeListener;
class MenuRunnerImpl;
}

namespace test {
class MenuRunnerTestAPI;
}

class VIEWS_EXPORT MenuRunner {
 public:
  enum RunTypes {
    
    HAS_MNEMONICS = 1 << 0,

    
    
    IS_NESTED     = 1 << 1,

    
    
    
    FOR_DROP      = 1 << 2,

    
    
    CONTEXT_MENU  = 1 << 3,

    
    
    COMBOBOX  = 1 << 4,
  };

  enum RunResult {
    
    MENU_DELETED,

    
    NORMAL_EXIT
  };

  
  explicit MenuRunner(ui::MenuModel* menu_model);
  explicit MenuRunner(MenuItemView* menu);
  ~MenuRunner();

  
  MenuItemView* GetMenu();

  
  
  
  void OwnMenu(MenuItemView* menu);

  
  
  
  
  
  
  RunResult RunMenuAt(Widget* parent,
                      MenuButton* button,
                      const gfx::Rect& bounds,
                      MenuItemView::AnchorPosition anchor,
                      ui::MenuSourceType source_type,
                      int32 types) WARN_UNUSED_RESULT;

  
  bool IsRunning() const;

  
  void Cancel();

  
  base::TimeDelta closing_event_time() const;

 private:
  friend class test::MenuRunnerTestAPI;

  
  void SetRunnerHandler(scoped_ptr<MenuRunnerHandler> runner_handler);

  scoped_ptr<MenuModelAdapter> menu_model_adapter_;

  internal::MenuRunnerImpl* holder_;

  
  
  scoped_ptr<MenuRunnerHandler> runner_handler_;

  scoped_ptr<internal::DisplayChangeListener> display_change_listener_;

  DISALLOW_COPY_AND_ASSIGN(MenuRunner);
};

namespace internal {

class DisplayChangeListener {
 public:
  virtual ~DisplayChangeListener() {}

  
  
  static DisplayChangeListener* Create(Widget* parent,
                                       MenuRunner* runner);

 protected:
  DisplayChangeListener() {}
};

}

}  

#endif  
