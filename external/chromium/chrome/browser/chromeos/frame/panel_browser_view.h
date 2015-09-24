// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FRAME_PANEL_BROWSER_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_FRAME_PANEL_BROWSER_VIEW_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/frame/panel_controller.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "ui/base/x/x11_util.h"

class Browser;

namespace chromeos {

class PanelController;

class PanelBrowserView : public ::BrowserView,
                         public PanelController::Delegate {
 public:
  explicit PanelBrowserView(Browser* browser);

  
  virtual void Show();
  virtual void ShowInactive();
  virtual void SetBounds(const gfx::Rect& bounds);
  virtual void Close();
  virtual void UpdateTitleBar();
  virtual void SetCreatorView(PanelBrowserView* creator);
  virtual bool GetSavedWindowBounds(gfx::Rect* bounds) const;
  virtual void OnWindowActivationChanged(bool active);

  
  virtual void TabChangedAt(TabContentsWrapper* contents,
                            int index,
                            TabChangeType change_type);

  
  virtual string16 GetPanelTitle();
  virtual SkBitmap GetPanelIcon();
  virtual bool CanClosePanel();
  virtual void ClosePanel();
  virtual void ActivatePanel();
  virtual void OnPanelStateChanged(PanelController::State state) {}

 private:
  
  void LimitBounds(gfx::Rect* bounds) const;

  void InitPanelController(bool is_active);

  
  scoped_ptr<chromeos::PanelController> panel_controller_;

  
  
  
  XID creator_xid_;
  DISALLOW_COPY_AND_ASSIGN(PanelBrowserView);
};

}  

#endif  
