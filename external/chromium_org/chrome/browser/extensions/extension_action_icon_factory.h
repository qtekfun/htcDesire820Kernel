// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_ACTION_ICON_FACTORY_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_ACTION_ICON_FACTORY_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/extension_icon_image.h"

class ExtensionAction;
class ExtensionIconSet;
class Profile;

namespace extensions {
class Extension;
}

class ExtensionActionIconFactory : public extensions::IconImage::Observer {
 public:
  class Observer {
   public:
    virtual ~Observer() {}
    
    virtual void OnIconUpdated() = 0;
  };

  
  ExtensionActionIconFactory(Profile* profile,
                             const extensions::Extension* extension,
                             const ExtensionAction* action,
                             Observer* observer);
  virtual ~ExtensionActionIconFactory();

  
  virtual void OnExtensionIconImageChanged(
      extensions::IconImage* image) OVERRIDE;

  
  
  
  
  
  
  
  
  gfx::Image GetIcon(int tab_id);

 private:
  
  
  gfx::ImageSkia GetBaseIconFromAction(int tab_id);

  const extensions::Extension* extension_;
  const ExtensionAction* action_;
  Observer* observer_;
  
  scoped_ptr<extensions::IconImage> default_icon_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionActionIconFactory);
};

#endif  
