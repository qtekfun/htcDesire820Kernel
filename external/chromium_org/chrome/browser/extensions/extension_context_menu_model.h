// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_CONTEXT_MENU_MODEL_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_CONTEXT_MENU_MODEL_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/extension_uninstall_dialog.h"
#include "ui/base/models/simple_menu_model.h"

class Browser;
class ExtensionAction;
class Profile;

namespace extensions {
class Extension;
}

class ExtensionContextMenuModel
    : public base::RefCounted<ExtensionContextMenuModel>,
      public ui::SimpleMenuModel,
      public ui::SimpleMenuModel::Delegate,
      public ExtensionUninstallDialog::Delegate {
 public:
  enum MenuEntries {
    NAME = 0,
    CONFIGURE,
    HIDE,
    UNINSTALL,
    MANAGE,
    INSPECT_POPUP
  };

  
  class PopupDelegate {
   public:
    
    
    virtual void InspectPopup(ExtensionAction* action) = 0;

   protected:
    virtual ~PopupDelegate() {}
  };

  
  
  
  
  ExtensionContextMenuModel(const extensions::Extension* extension,
                            Browser* browser,
                            PopupDelegate* delegate);

  
  
  ExtensionContextMenuModel(const extensions::Extension* extension,
                            Browser* browser);

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  
  virtual void ExtensionUninstallAccepted() OVERRIDE;
  virtual void ExtensionUninstallCanceled() OVERRIDE;

 private:
  friend class base::RefCounted<ExtensionContextMenuModel>;
  virtual ~ExtensionContextMenuModel();

  void InitMenu(const extensions::Extension* extension);

  
  
  const extensions::Extension* GetExtension() const;

  
  std::string extension_id_;

  
  
  ExtensionAction* extension_action_;

  Browser* browser_;

  Profile* profile_;

  
  PopupDelegate* delegate_;

  
  scoped_ptr<ExtensionUninstallDialog> extension_uninstall_dialog_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionContextMenuModel);
};

#endif  
