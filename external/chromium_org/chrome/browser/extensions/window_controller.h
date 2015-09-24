// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_EXTENSIONS_WINDOW_CONTROLLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"

class Browser;  
class GURL;
class Profile;
class SessionID;

namespace base {
class DictionaryValue;
}

namespace gfx {
class Rect;
}

namespace ui {
class BaseWindow;
}

namespace extensions {
class Extension;

class WindowController {
 public:
  enum Reason {
    REASON_NONE,
    REASON_NOT_EDITABLE,
  };

  WindowController(ui::BaseWindow* window, Profile* profile);
  virtual ~WindowController();

  ui::BaseWindow* window() const { return window_; }

  Profile* profile() const { return profile_; }

  
  virtual int GetWindowId() const = 0;

  
  virtual std::string GetWindowTypeText() const = 0;

  
  
  
  virtual base::DictionaryValue* CreateWindowValue() const;

  
  virtual base::DictionaryValue* CreateWindowValueWithTabs(
      const extensions::Extension* extension) const = 0;

  virtual base::DictionaryValue* CreateTabValue(
      const extensions::Extension* extension, int tab_index) const = 0;

  
  
  virtual bool CanClose(Reason* reason) const = 0;

  
  
  virtual void SetFullscreenMode(bool is_fullscreen,
                                 const GURL& extension_url) const = 0;

  
  
  virtual Browser* GetBrowser() const;

  
  
  virtual bool IsVisibleToExtension(const Extension* extension) const = 0;

 private:
  ui::BaseWindow* window_;
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(WindowController);
};

}  

#endif  
