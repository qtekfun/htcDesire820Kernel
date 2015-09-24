// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_FIND_BAR_FIND_BAR_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_FIND_BAR_FIND_BAR_BRIDGE_H_

#include "base/compiler_specific.h"
#include "base/logging.h"
#include "chrome/browser/ui/find_bar/find_bar.h"

class Browser;
class FindBarController;

#ifdef __OBJC__
@class FindBarCocoaController;
#else
class FindBarCocoaController;
#endif


class FindBarBridge : public FindBar,
                      public FindBarTesting {
 public:
  FindBarBridge(Browser* browser);
  virtual ~FindBarBridge();

  FindBarCocoaController* find_bar_cocoa_controller() {
    return cocoa_controller_;
  }

  virtual void SetFindBarController(
      FindBarController* find_bar_controller) OVERRIDE;

  virtual FindBarController* GetFindBarController() const OVERRIDE;

  virtual FindBarTesting* GetFindBarTesting() OVERRIDE;

  
  virtual void Show(bool animate) OVERRIDE;
  virtual void Hide(bool animate) OVERRIDE;
  virtual void SetFocusAndSelection() OVERRIDE;
  virtual void ClearResults(const FindNotificationDetails& results) OVERRIDE;
  virtual void StopAnimation() OVERRIDE;
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
  virtual void MoveWindowIfNecessary(const gfx::Rect& selection_rect,
                                     bool no_redraw) OVERRIDE;

  
  virtual bool GetFindBarWindowInfo(gfx::Point* position,
                                    bool* fully_visible) OVERRIDE;
  virtual base::string16 GetFindSelectedText() OVERRIDE;
  virtual base::string16 GetMatchCountText() OVERRIDE;
  virtual int GetWidth() OVERRIDE;

  
  static bool disable_animations_during_testing_;

 private:
  
  
  FindBarCocoaController* cocoa_controller_;

  
  FindBarController* find_bar_controller_;  

  DISALLOW_COPY_AND_ASSIGN(FindBarBridge);
};

#endif  
