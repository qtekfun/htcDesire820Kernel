// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_CONTEXT_MENU_MODEL_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_CONTEXT_MENU_MODEL_H_
#pragma once

#include <string>

#include "chrome/browser/extensions/extension_uninstall_dialog.h"
#include "ui/base/models/simple_menu_model.h"

class Browser;
class Extension;
class ExtensionAction;
class Profile;

class ExtensionContextMenuModel
    : public base::RefCounted<ExtensionContextMenuModel>,
      public ui::SimpleMenuModel,
      public ui::SimpleMenuModel::Delegate,
      public ExtensionUninstallDialog::Delegate {
 public:
  
  class PopupDelegate {
   public:
    
    
    virtual void InspectPopup(ExtensionAction* action) = 0;

   protected:
    virtual ~PopupDelegate() {}
  };

  
  
  
  
  ExtensionContextMenuModel(const Extension* extension,
                            Browser* browser,
                            PopupDelegate* delegate);
  virtual ~ExtensionContextMenuModel();

  
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator);
  virtual void ExecuteCommand(int command_id);

  
  virtual void ExtensionDialogAccepted();
  virtual void ExtensionDialogCanceled();

 private:
  void InitCommonCommands();

  
  
  const Extension* GetExtension() const;

  
  std::string extension_id_;

  
  ExtensionAction* extension_action_;

  Browser* browser_;

  Profile* profile_;

  
  PopupDelegate* delegate_;

  
  scoped_ptr<ExtensionUninstallDialog> extension_uninstall_dialog_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionContextMenuModel);
};

#endif  
