// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_RENDER_VIEW_CONTEXT_MENU_H_
#define CHROME_BROWSER_TAB_CONTENTS_RENDER_VIEW_CONTEXT_MENU_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "chrome/browser/custom_handlers/protocol_handler_registry.h"
#include "chrome/browser/extensions/context_menu_matcher.h"
#include "chrome/browser/extensions/menu_manager.h"
#include "chrome/browser/tab_contents/render_view_context_menu_observer.h"
#include "content/public/common/context_menu_params.h"
#include "content/public/common/page_transition_types.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/window_open_disposition.h"

class PrintPreviewContextMenuObserver;
class Profile;
class SpellingMenuObserver;
class SpellCheckerSubMenuObserver;

namespace content {
class RenderViewHost;
class WebContents;
}

namespace extensions {
class Extension;
class MenuItem;
}

namespace gfx {
class Point;
}

namespace blink {
struct WebMediaPlayerAction;
struct WebPluginAction;
}

class RenderViewContextMenuProxy {
 public:
  
  virtual void AddMenuItem(int command_id, const base::string16& title) = 0;
  virtual void AddCheckItem(int command_id, const base::string16& title) = 0;
  virtual void AddSeparator() = 0;

  
  virtual void AddSubMenu(int command_id,
                          const base::string16& label,
                          ui::MenuModel* model) = 0;

  
  virtual void UpdateMenuItem(int command_id,
                              bool enabled,
                              bool hidden,
                              const base::string16& title) = 0;

  
  virtual content::RenderViewHost* GetRenderViewHost() const = 0;
  virtual content::WebContents* GetWebContents() const = 0;
  virtual Profile* GetProfile() const = 0;
};

class RenderViewContextMenu : public ui::SimpleMenuModel::Delegate,
                              public RenderViewContextMenuProxy {
 public:
  static const size_t kMaxSelectionTextLength;

  RenderViewContextMenu(content::WebContents* web_contents,
                        const content::ContextMenuParams& params);

  virtual ~RenderViewContextMenu();

  
  void Init();

  
  void Cancel();

  
  const ui::MenuModel& menu_model() const { return menu_model_; }

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;
  virtual void MenuWillShow(ui::SimpleMenuModel* source) OVERRIDE;
  virtual void MenuClosed(ui::SimpleMenuModel* source) OVERRIDE;

  
  virtual void AddMenuItem(int command_id,
                           const base::string16& title) OVERRIDE;
  virtual void AddCheckItem(int command_id,
                            const base::string16& title) OVERRIDE;
  virtual void AddSeparator() OVERRIDE;
  virtual void AddSubMenu(int command_id,
                          const base::string16& label,
                          ui::MenuModel* model) OVERRIDE;
  virtual void UpdateMenuItem(int command_id,
                              bool enabled,
                              bool hidden,
                              const base::string16& title) OVERRIDE;
  virtual content::RenderViewHost* GetRenderViewHost() const OVERRIDE;
  virtual content::WebContents* GetWebContents() const OVERRIDE;
  virtual Profile* GetProfile() const OVERRIDE;

 protected:
  void InitMenu();

  
  virtual void PlatformInit() = 0;
  virtual void PlatformCancel() = 0;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) = 0;
  virtual void AppendPlatformEditableItems();

  content::ContextMenuParams params_;
  content::WebContents* source_web_contents_;
  Profile* profile_;

  ui::SimpleMenuModel menu_model_;
  extensions::ContextMenuMatcher extension_items_;

  
  bool external_;

 private:
  friend class RenderViewContextMenuTest;
  friend class RenderViewContextMenuPrefsTest;

  static bool IsDevToolsURL(const GURL& url);
  static bool IsInternalResourcesURL(const GURL& url);
  static bool ExtensionContextAndPatternMatch(
      const content::ContextMenuParams& params,
      extensions::MenuItem::ContextList contexts,
      const extensions::URLPatternSet& target_url_patterns);
  static bool MenuItemMatchesParams(
      const content::ContextMenuParams& params,
      const extensions::MenuItem* item);

  
  const extensions::Extension* GetExtension() const;
  void AppendAppModeItems();
  void AppendPlatformAppItems();
  void AppendPopupExtensionItems();
  void AppendPanelItems();
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
  void AppendPrintItem();
  void AppendEditableItems();
  void AppendSearchProvider();
  void AppendAllExtensionItems();
  void AppendSpellingSuggestionsSubMenu();
  void AppendSpellcheckOptionsSubMenu();
  void AppendSpeechInputOptionsSubMenu();
  void AppendProtocolHandlerSubMenu();

  
  
  
  void OpenURL(const GURL& url, const GURL& referrer, int64 frame_id,
               WindowOpenDisposition disposition,
               content::PageTransition transition);

  
  void CopyImageAt(int x, int y);

  
  void GetImageThumbnailForSearch();

  
  void Inspect(int x, int y);

  
  void WriteURLToClipboard(const GURL& url);

  void MediaPlayerActionAt(const gfx::Point& location,
                           const blink::WebMediaPlayerAction& action);
  void PluginActionAt(const gfx::Point& location,
                      const blink::WebPluginAction& action);

  bool IsDevCommandEnabled(int id) const;

  
  
  ProtocolHandlerRegistry::ProtocolHandlerList GetHandlersForLinkUrl();

  
  
  base::string16 PrintableSelectionText();

  
  
  GURL selection_navigation_url_;

  ui::SimpleMenuModel speech_input_submenu_model_;
  ui::SimpleMenuModel protocol_handler_submenu_model_;
  ScopedVector<ui::SimpleMenuModel> extension_menu_models_;
  ProtocolHandlerRegistry* protocol_handler_registry_;

  
  scoped_ptr<SpellingMenuObserver> spelling_menu_observer_;

  
  scoped_ptr<SpellCheckerSubMenuObserver> spellchecker_submenu_observer_;

#if defined(ENABLE_FULL_PRINTING)
  
  scoped_ptr<PrintPreviewContextMenuObserver> print_preview_menu_observer_;
#endif

  
  mutable ObserverList<RenderViewContextMenuObserver> observers_;

  
  
  bool command_executed_;

  
  
  
  
  
  bool is_guest_;

  DISALLOW_COPY_AND_ASSIGN(RenderViewContextMenu);
};

#endif  
