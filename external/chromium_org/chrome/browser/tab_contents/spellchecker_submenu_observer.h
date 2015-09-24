// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_SPELLCHECKER_SUBMENU_OBSERVER_H_
#define CHROME_BROWSER_TAB_CONTENTS_SPELLCHECKER_SUBMENU_OBSERVER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/tab_contents/render_view_context_menu_observer.h"
#include "ui/base/models/simple_menu_model.h"

class RenderViewContextMenuProxy;

class SpellCheckerSubMenuObserver : public RenderViewContextMenuObserver {
 public:
  SpellCheckerSubMenuObserver(RenderViewContextMenuProxy* proxy,
                              ui::SimpleMenuModel::Delegate* delegate,
                              int group);
  virtual ~SpellCheckerSubMenuObserver();

  
  virtual void InitMenu(const content::ContextMenuParams& params) OVERRIDE;
  virtual bool IsCommandIdSupported(int command_id) OVERRIDE;
  virtual bool IsCommandIdChecked(int command_id) OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) OVERRIDE;
  virtual void ExecuteCommand(int command_id) OVERRIDE;

 private:
  
  RenderViewContextMenuProxy* proxy_;

  
  
  ui::SimpleMenuModel submenu_model_;

#if !defined(OS_MACOSX)
  
  
  int language_group_;
  int language_selected_;
  std::vector<std::string> languages_;
#endif  

  DISALLOW_COPY_AND_ASSIGN(SpellCheckerSubMenuObserver);
};

#endif  
