// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FIND_BAR_HOST_H_
#define CHROME_BROWSER_UI_VIEWS_FIND_BAR_HOST_H_

#include "base/compiler_specific.h"
#include "chrome/browser/ui/find_bar/find_bar.h"
#include "chrome/browser/ui/views/dropdown_bar_host.h"
#include "chrome/browser/ui/views/find_bar_view.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/views/controls/textfield/textfield.h"

class BrowserView;
class FindBarController;
class FindNotificationDetails;

class FindBarHost : public DropdownBarHost,
                    public FindBar,
                    public FindBarTesting {
 public:
  explicit FindBarHost(BrowserView* browser_view);
  virtual ~FindBarHost();

  
  
  
  
  bool MaybeForwardKeyEventToWebpage(const ui::KeyEvent& key_event);

  
  virtual FindBarController* GetFindBarController() const OVERRIDE;
  virtual void SetFindBarController(
      FindBarController* find_bar_controller) OVERRIDE;
  virtual void Show(bool animate) OVERRIDE;
  virtual void Hide(bool animate) OVERRIDE;
  virtual void SetFocusAndSelection() OVERRIDE;
  virtual void ClearResults(const FindNotificationDetails& results) OVERRIDE;
  virtual void StopAnimation() OVERRIDE;
  virtual void MoveWindowIfNecessary(const gfx::Rect& selection_rect,
                                     bool no_redraw) OVERRIDE;
  virtual void SetFindTextAndSelectedRange(
      const base::string16& find_text,
      const gfx::Range& selected_range) OVERRIDE;
  virtual base::string16 GetFindText() OVERRIDE;
  virtual gfx::Range GetSelectedRange() OVERRIDE;
  virtual void UpdateUIForFindResult(const FindNotificationDetails& result,
                                     const base::string16& find_text) OVERRIDE;
  virtual void AudibleAlert() OVERRIDE;
  virtual bool IsFindBarVisible() OVERRIDE;
  virtual void RestoreSavedFocus() OVERRIDE;
  virtual bool HasGlobalFindPasteboard() OVERRIDE;
  virtual void UpdateFindBarForChangedWebContents() OVERRIDE;
  virtual FindBarTesting* GetFindBarTesting() OVERRIDE;

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual bool CanHandleAccelerators() const OVERRIDE;

  
  virtual bool GetFindBarWindowInfo(gfx::Point* position,
                                    bool* fully_visible) OVERRIDE;
  virtual base::string16 GetFindSelectedText() OVERRIDE;
  virtual base::string16 GetMatchCountText() OVERRIDE;
  virtual int GetWidth() OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  virtual gfx::Rect GetDialogPosition(
      gfx::Rect avoid_overlapping_rect) OVERRIDE;
  
  
  
  
  
  virtual void SetDialogPosition(const gfx::Rect& new_pos,
                                 bool no_redraw) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  virtual void GetWidgetBounds(gfx::Rect* bounds) OVERRIDE;

  
  virtual void RegisterAccelerators() OVERRIDE;
  virtual void UnregisterAccelerators() OVERRIDE;

 protected:
  
  virtual void OnVisibilityChanged() OVERRIDE;

 private:
  
  void GetWidgetPositionNative(gfx::Rect* avoid_overlapping_rect);

  
  bool ShouldForwardKeyEventToWebpageNative(const ui::KeyEvent& key_event);

  
  FindBarView* find_bar_view() { return static_cast<FindBarView*>(view()); }

  
  FindBarController* find_bar_controller_;

  DISALLOW_COPY_AND_ASSIGN(FindBarHost);
};

#endif  
