// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_FOCUS_FOCUS_MANAGER_TEST_H_
#define UI_VIEWS_FOCUS_FOCUS_MANAGER_TEST_H_

#include "ui/views/focus/focus_manager.h"
#include "ui/views/focus/widget_focus_manager.h"
#include "ui/views/test/views_test_base.h"
#include "ui/views/widget/widget_delegate.h"

namespace views {

class FocusChangeListener;

class FocusManagerTest : public ViewsTestBase, public WidgetDelegate {
 public:
  FocusManagerTest();
  virtual ~FocusManagerTest();

  
  FocusManager* GetFocusManager();

  
  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  
  virtual View* GetContentsView() OVERRIDE;
  virtual Widget* GetWidget() OVERRIDE;
  virtual const Widget* GetWidget() const OVERRIDE;
  virtual void GetAccessiblePanes(std::vector<View*>* panes) OVERRIDE;

 protected:
  
  
  virtual void InitContentView();

  void AddFocusChangeListener(FocusChangeListener* listener);
  void AddWidgetFocusChangeListener(WidgetFocusChangeListener* listener);

  
  void SetAccessiblePanes(const std::vector<View*>& panes);

#if defined(OS_WIN) && !defined(USE_AURA)
  
  void SimulateActivateWindow();
  void SimulateDeactivateWindow();

  void PostKeyDown(ui::KeyboardCode key_code);
  void PostKeyUp(ui::KeyboardCode key_code);
#endif

 private:
  View* contents_view_;
  FocusChangeListener* focus_change_listener_;
  WidgetFocusChangeListener* widget_focus_change_listener_;
  std::vector<View*> accessible_panes_;

  DISALLOW_COPY_AND_ASSIGN(FocusManagerTest);
};

typedef std::pair<View*, View*> ViewPair;

class TestFocusChangeListener : public FocusChangeListener {
 public:
  TestFocusChangeListener();
  virtual ~TestFocusChangeListener();

  const std::vector<ViewPair>& focus_changes() const { return focus_changes_; }
  void ClearFocusChanges();

  
  virtual void OnWillChangeFocus(View* focused_before,
                                 View* focused_now) OVERRIDE;
  virtual void OnDidChangeFocus(View* focused_before,
                                View* focused_now) OVERRIDE;

 private:
  
  std::vector<ViewPair> focus_changes_;

  DISALLOW_COPY_AND_ASSIGN(TestFocusChangeListener);
};

typedef std::pair<gfx::NativeView, gfx::NativeView> NativeViewPair;

class TestWidgetFocusChangeListener : public WidgetFocusChangeListener {
 public:
  TestWidgetFocusChangeListener();
  virtual ~TestWidgetFocusChangeListener();

  const std::vector<NativeViewPair>& focus_changes() const {
    return focus_changes_;
  }
  void ClearFocusChanges();

  
  virtual void OnNativeFocusChange(gfx::NativeView focused_before,
                                   gfx::NativeView focused_now) OVERRIDE;

 private:
  
  
  std::vector<NativeViewPair> focus_changes_;

  DISALLOW_COPY_AND_ASSIGN(TestWidgetFocusChangeListener);
};

}  

#endif  
