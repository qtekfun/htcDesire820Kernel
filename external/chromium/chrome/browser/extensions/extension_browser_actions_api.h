// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_BROWSER_ACTIONS_API_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_BROWSER_ACTIONS_API_H_
#pragma once

#include "chrome/browser/extensions/extension_function.h"
#include "chrome/common/extensions/extension_action.h"

class DictionaryValue;
class ExtensionAction;

class BrowserActionFunction : public SyncExtensionFunction {
 protected:
  BrowserActionFunction()
      : details_(NULL),
        tab_id_(ExtensionAction::kDefaultTabId),
        browser_action_(NULL) {}
  virtual ~BrowserActionFunction() {}
  virtual bool RunImpl();
  virtual bool RunBrowserAction() = 0;

  
  
  DictionaryValue* details_;

  
  
  int tab_id_;

  
  ExtensionAction* browser_action_;
};

class BrowserActionSetIconFunction : public BrowserActionFunction {
  ~BrowserActionSetIconFunction() {}
  virtual bool RunBrowserAction();
  DECLARE_EXTENSION_FUNCTION_NAME("browserAction.setIcon")
};

class BrowserActionSetTitleFunction : public BrowserActionFunction {
  ~BrowserActionSetTitleFunction() {}
  virtual bool RunBrowserAction();
  DECLARE_EXTENSION_FUNCTION_NAME("browserAction.setTitle")
};

class BrowserActionSetPopupFunction : public BrowserActionFunction {
  ~BrowserActionSetPopupFunction() {}
  virtual bool RunBrowserAction();
  DECLARE_EXTENSION_FUNCTION_NAME("browserAction.setPopup")
};

class BrowserActionSetBadgeTextFunction : public BrowserActionFunction {
  ~BrowserActionSetBadgeTextFunction() {}
  virtual bool RunBrowserAction();
  DECLARE_EXTENSION_FUNCTION_NAME("browserAction.setBadgeText")
};

class BrowserActionSetBadgeBackgroundColorFunction
    : public BrowserActionFunction {
  ~BrowserActionSetBadgeBackgroundColorFunction() {}
  virtual bool RunBrowserAction();
  DECLARE_EXTENSION_FUNCTION_NAME("browserAction.setBadgeBackgroundColor")
};

#endif  
