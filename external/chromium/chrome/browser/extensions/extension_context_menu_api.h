// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_CONTEXT_MENU_API_H__
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_CONTEXT_MENU_API_H__
#pragma once

#include "chrome/browser/extensions/extension_function.h"
#include "chrome/browser/extensions/extension_menu_manager.h"
#include "chrome/common/extensions/extension_extent.h"

class DictionaryValue;
class ExtensionMenuItem;

class ExtensionContextMenuFunction : public SyncExtensionFunction {
 public:
  ~ExtensionContextMenuFunction() {}

 protected:
  
  bool ParseContexts(const DictionaryValue& properties,
                     const char* key,
                     ExtensionMenuItem::ContextList* result);

  
  
  
  
  bool ParseType(const DictionaryValue& properties,
                 const ExtensionMenuItem::Type& default_value,
                 ExtensionMenuItem::Type* result);

  
  bool ParseChecked(ExtensionMenuItem::Type type,
                    const DictionaryValue& properties,
                    bool default_value,
                    bool* checked);

  
  
  bool ParseURLPatterns(const DictionaryValue& properties,
                        const char* key,
                        ExtensionExtent* result);

  
  
  bool SetURLPatterns(const DictionaryValue& properties,
                      ExtensionMenuItem* item);

  
  
  // on error, with an explanation written into error_. Note that if the
  
  
  
  bool GetParent(const DictionaryValue& properties,
                 const ExtensionMenuManager& manager,
                 ExtensionMenuItem** result);
};

class CreateContextMenuFunction : public ExtensionContextMenuFunction {
  ~CreateContextMenuFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("contextMenus.create")
};

class UpdateContextMenuFunction : public ExtensionContextMenuFunction {
  ~UpdateContextMenuFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("contextMenus.update")
};

class RemoveContextMenuFunction : public ExtensionContextMenuFunction {
  ~RemoveContextMenuFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("contextMenus.remove")
};

class RemoveAllContextMenusFunction : public ExtensionContextMenuFunction {
  ~RemoveAllContextMenusFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("contextMenus.removeAll")
};

#endif  
