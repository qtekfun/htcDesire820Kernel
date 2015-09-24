// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_FIND_BAR_FIND_BAR_H_
#define CHROME_BROWSER_UI_FIND_BAR_FIND_BAR_H_

#include "base/strings/string16.h"
#include "ui/gfx/rect.h"

class FindBarController;
class FindBarTesting;
class FindNotificationDetails;

namespace gfx {
class Range;
}

class FindBar {
 public:
  virtual ~FindBar() { }

  
  virtual FindBarController* GetFindBarController() const = 0;
  virtual void SetFindBarController(
      FindBarController* find_bar_controller) = 0;

  
  
  virtual void Show(bool animate) = 0;

  
  
  virtual void Hide(bool animate) = 0;

  
  virtual void SetFocusAndSelection() = 0;

  
  virtual void ClearResults(const FindNotificationDetails& results) = 0;

  
  virtual void StopAnimation() = 0;

  
  
  
  
  
  
  
  virtual void MoveWindowIfNecessary(const gfx::Rect& selection_rect,
                                     bool no_redraw) = 0;

  
  virtual void SetFindTextAndSelectedRange(
      const base::string16& find_text,
      const gfx::Range& selected_range) = 0;

  
  virtual base::string16 GetFindText() = 0;

  
  virtual gfx::Range GetSelectedRange() = 0;

  
  
  virtual void UpdateUIForFindResult(const FindNotificationDetails& result,
                                     const base::string16& find_text) = 0;

  
  virtual void AudibleAlert() = 0;

  virtual bool IsFindBarVisible() = 0;

  
  
  virtual void RestoreSavedFocus() = 0;

  
  virtual bool HasGlobalFindPasteboard() = 0;

  
  virtual void UpdateFindBarForChangedWebContents() = 0;

  
  
  virtual FindBarTesting* GetFindBarTesting() = 0;
};

class FindBarTesting {
 public:
  virtual ~FindBarTesting() { }

  
  
  
  
  
  
  virtual bool GetFindBarWindowInfo(gfx::Point* position,
                                    bool* fully_visible) = 0;

  
  virtual base::string16 GetFindSelectedText() = 0;

  
  virtual base::string16 GetMatchCountText() = 0;

  
  virtual int GetWidth() = 0;
};

#endif  
