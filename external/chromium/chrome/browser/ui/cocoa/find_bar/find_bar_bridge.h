// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_FIND_BAR_FIND_BAR_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_FIND_BAR_FIND_BAR_BRIDGE_H_
#pragma once

#include "base/logging.h"
#include "chrome/browser/ui/find_bar/find_bar.h"

class FindBarController;

#ifdef __OBJC__
@class FindBarCocoaController;
#else
class FindBarCocoaController;
#endif


class FindBarBridge : public FindBar,
                      public FindBarTesting {
 public:
  FindBarBridge();
  virtual ~FindBarBridge();

  FindBarCocoaController* find_bar_cocoa_controller() {
    return cocoa_controller_;
  }

  virtual void SetFindBarController(FindBarController* find_bar_controller);

  virtual FindBarController* GetFindBarController() const;

  virtual FindBarTesting* GetFindBarTesting();

  
  virtual void Show(bool animate);
  virtual void Hide(bool animate);
  virtual void SetFocusAndSelection();
  virtual void ClearResults(const FindNotificationDetails& results);
  virtual void StopAnimation();
  virtual void SetFindText(const string16& find_text);
  virtual void UpdateUIForFindResult(const FindNotificationDetails& result,
                                     const string16& find_text);
  virtual void AudibleAlert();
  virtual bool IsFindBarVisible();
  virtual void RestoreSavedFocus();
  virtual void MoveWindowIfNecessary(const gfx::Rect& selection_rect,
                                     bool no_redraw);

  
  virtual bool GetFindBarWindowInfo(gfx::Point* position,
                                    bool* fully_visible);
  virtual string16 GetFindText();
  virtual string16 GetFindSelectedText();
  virtual string16 GetMatchCountText();

  
  static bool disable_animations_during_testing_;

 private:
  
  
  FindBarCocoaController* cocoa_controller_;

  
  FindBarController* find_bar_controller_;  

  DISALLOW_COPY_AND_ASSIGN(FindBarBridge);
};

#endif  
