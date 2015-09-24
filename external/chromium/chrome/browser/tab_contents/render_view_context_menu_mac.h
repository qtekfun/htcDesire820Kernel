// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_RENDER_VIEW_CONTEXT_MENU_MAC_H_
#define CHROME_BROWSER_TAB_CONTENTS_RENDER_VIEW_CONTEXT_MENU_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/tab_contents/render_view_context_menu.h"

@class MenuController;


class RenderViewContextMenuMac : public RenderViewContextMenu {
 public:
  RenderViewContextMenuMac(TabContents* web_contents,
                           const ContextMenuParams& params,
                           NSView* parent_view);
  virtual ~RenderViewContextMenuMac();
  virtual void ExecuteCommand(int id);

 protected:
  
  virtual void PlatformInit();
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator);

  virtual void LookUpInDictionary();

  void InitPlatformMenu();
 private:
  scoped_nsobject<MenuController> menuController_;
  NSView* parent_view_;  
};

#endif  
