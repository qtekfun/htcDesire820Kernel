// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_RENDER_VIEW_CONTEXT_MENU_H_
#define CHROME_BROWSER_TAB_CONTENTS_RENDER_VIEW_CONTEXT_MENU_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_vector.h"
#include "base/string16.h"
#include "chrome/browser/extensions/extension_menu_manager.h"
#include "content/common/page_transition_types.h"
#include "ui/base/models/simple_menu_model.h"
#include "webkit/glue/context_menu.h"
#include "webkit/glue/window_open_disposition.h"

class ExtensionMenuItem;
class Profile;
class TabContents;

namespace gfx {
class Point;
}

namespace WebKit {
struct WebMediaPlayerAction;
}

class RenderViewContextMenu : public ui::SimpleMenuModel::Delegate {
 public:
  static const size_t kMaxExtensionItemTitleLength;
  static const size_t kMaxSelectionTextLength;

  RenderViewContextMenu(TabContents* tab_contents,
                        const ContextMenuParams& params);

  virtual ~RenderViewContextMenu();

  
  void Init();

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual void ExecuteCommand(int command_id) OVERRIDE;
  virtual void MenuWillShow() OVERRIDE;
  virtual void MenuClosed() OVERRIDE;

 protected:
  void InitMenu();

  
  virtual void PlatformInit() = 0;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) = 0;
  virtual void LookUpInDictionary();

  
  ExtensionMenuItem* GetExtensionMenuItem(int id) const;

  ContextMenuParams params_;
  TabContents* source_tab_contents_;
  Profile* profile_;

  ui::SimpleMenuModel menu_model_;

  
  bool external_;


  
  
  std::map<int, ExtensionMenuItem::Id> extension_item_map_;

 private:
  static bool IsDevToolsURL(const GURL& url);
  static bool IsInternalResourcesURL(const GURL& url);
  bool AppendCustomItems();
  void AppendDeveloperItems();
  void AppendLinkItems();
  void AppendImageItems();
  void AppendAudioItems();
  void AppendVideoItems();
  void AppendMediaItems();
  void AppendPluginItems();
  void AppendPageItems();
  void AppendFrameItems();
  void AppendCopyItem();
  void AppendEditableItems();
  void AppendSearchProvider();
  void AppendAllExtensionItems();
  void AppendSpellcheckOptionsSubMenu();
  
  void AppendBidiSubMenu();

  
  
  
  void AppendExtensionItems(const std::string& extension_id, int* index);

  
  void RecursivelyAppendExtensionItems(
      const std::vector<ExtensionMenuItem*>& items,
      bool can_cross_incognito,
      ui::SimpleMenuModel* menu_model,
      int *index);
  
  void SetExtensionIcon(const std::string& extension_id);

  
  
  void OpenURL(const GURL& url,
               WindowOpenDisposition disposition,
               PageTransition::Type transition);

  
  void CopyImageAt(int x, int y);

  
  void Inspect(int x, int y);

  
  void WriteURLToClipboard(const GURL& url);

  void MediaPlayerActionAt(const gfx::Point& location,
                           const WebKit::WebMediaPlayerAction& action);

  bool IsDevCommandEnabled(int id) const;

  
  
  string16 PrintableSelectionText();

  
  
  GURL selection_navigation_url_;

  ui::SimpleMenuModel spellcheck_submenu_model_;
  ui::SimpleMenuModel bidi_submenu_model_;
  ScopedVector<ui::SimpleMenuModel> extension_menu_models_;

  DISALLOW_COPY_AND_ASSIGN(RenderViewContextMenu);
};

#endif  
