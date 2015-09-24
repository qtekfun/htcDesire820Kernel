// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TAB_CONTENTS_RENDER_VIEW_CONTEXT_MENU_GTK_H_
#define CHROME_BROWSER_UI_GTK_TAB_CONTENTS_RENDER_VIEW_CONTEXT_MENU_GTK_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/tab_contents/render_view_context_menu.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "ui/gfx/point.h"

namespace content {
class RenderWidgetHostView;
struct ContextMenuParams;
}

class RenderViewContextMenuGtk : public RenderViewContextMenu,
                                 public MenuGtk::Delegate {
 public:
  RenderViewContextMenuGtk(content::WebContents* web_contents,
                           const content::ContextMenuParams& params,
                           content::RenderWidgetHostView* view);

  virtual ~RenderViewContextMenuGtk();

  
  void Popup(const gfx::Point& point);

  
  virtual bool AlwaysShowIconForCmd(int command_id) const OVERRIDE;

  
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
  
  void AppendBidiSubMenu();

  
  ui::SimpleMenuModel bidi_submenu_model_;

  scoped_ptr<MenuGtk> menu_gtk_;
  uint32_t triggering_event_time_;
};

#endif  
