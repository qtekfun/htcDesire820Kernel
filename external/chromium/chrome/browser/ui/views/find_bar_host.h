// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FIND_BAR_HOST_H_
#define CHROME_BROWSER_UI_VIEWS_FIND_BAR_HOST_H_
#pragma once

#include "chrome/browser/ui/find_bar/find_bar.h"
#include "chrome/browser/ui/views/dropdown_bar_host.h"
#include "content/browser/renderer_host/render_view_host_delegate.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "views/controls/textfield/textfield.h"

class BrowserView;
class FindBarController;
class FindBarView;
class FindNotificationDetails;

class FindBarHost : public DropdownBarHost,
                    public FindBar,
                    public FindBarTesting {
 public:
  explicit FindBarHost(BrowserView* browser_view);
  virtual ~FindBarHost();

  
  
  
  
  bool MaybeForwardKeyEventToWebpage(const views::KeyEvent& key_event);

  
  virtual FindBarController* GetFindBarController() const;
  virtual void SetFindBarController(FindBarController* find_bar_controller);
  virtual void Show(bool animate);
  virtual void Hide(bool animate);
  virtual void SetFocusAndSelection();
  virtual void ClearResults(const FindNotificationDetails& results);
  virtual void StopAnimation();
  virtual void MoveWindowIfNecessary(const gfx::Rect& selection_rect,
                                     bool no_redraw);
  virtual void SetFindText(const string16& find_text);
  virtual void UpdateUIForFindResult(const FindNotificationDetails& result,
                                     const string16& find_text);
  virtual void AudibleAlert();
  virtual bool IsFindBarVisible();
  virtual void RestoreSavedFocus();
  virtual FindBarTesting* GetFindBarTesting();

  
  virtual bool AcceleratorPressed(const views::Accelerator& accelerator);

  
  virtual bool GetFindBarWindowInfo(gfx::Point* position,
                                    bool* fully_visible);
  virtual string16 GetFindText();
  virtual string16 GetFindSelectedText();
  virtual string16 GetMatchCountText();

  
  
  
  
  
  
  
  
  
  
  
  
  virtual gfx::Rect GetDialogPosition(gfx::Rect avoid_overlapping_rect);
  
  
  
  
  
  virtual void SetDialogPosition(const gfx::Rect& new_pos, bool no_redraw);

  
  
  
  
  
  
  
  
  
  
  
  virtual void GetWidgetBounds(gfx::Rect* bounds);

  
  virtual void RegisterAccelerators();
  virtual void UnregisterAccelerators();

 private:
  
  void GetWidgetPositionNative(gfx::Rect* avoid_overlapping_rect);

  
  bool ShouldForwardKeyEventToWebpageNative(
      const views::KeyEvent& key_event);

  
  FindBarView* find_bar_view();

  
  FindBarController* find_bar_controller_;

  DISALLOW_COPY_AND_ASSIGN(FindBarHost);
};

#endif  
