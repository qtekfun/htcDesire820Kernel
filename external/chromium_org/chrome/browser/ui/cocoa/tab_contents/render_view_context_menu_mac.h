// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_RENDER_VIEW_CONTEXT_MENU_MAC_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_RENDER_VIEW_CONTEXT_MENU_MAC_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/tab_contents/render_view_context_menu.h"

@class MenuController;

class RenderViewContextMenuMac : public RenderViewContextMenu {
 public:
  RenderViewContextMenuMac(content::WebContents* web_contents,
                           const content::ContextMenuParams& params,
                           NSView* parent_view);
  virtual ~RenderViewContextMenuMac();

  
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;

  
  virtual void UpdateMenuItem(int command_id,
                              bool enabled,
                              bool hidden,
                              const base::string16& title) OVERRIDE;

 protected:
  
  virtual void PlatformInit() OVERRIDE;
  virtual void PlatformCancel() OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void AppendPlatformEditableItems() OVERRIDE;

 private:
  
  void InitPlatformMenu();

  
  void AppendBidiSubMenu();

  
  void LookUpInDictionary();

  
  void StartSpeaking();

  
  void StopSpeaking();

  
  base::scoped_nsobject<MenuController> menu_controller_;

  
  ui::SimpleMenuModel speech_submenu_model_;

  
  ui::SimpleMenuModel bidi_submenu_model_;

  
  NSView* parent_view_;

  DISALLOW_COPY_AND_ASSIGN(RenderViewContextMenuMac);
};

#endif  
