// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_UNINSTALL_DIALOG_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_UNINSTALL_DIALOG_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/gfx/image/image_skia.h"

class Browser;
class Profile;

namespace base {
class MessageLoop;
}

namespace extensions {
class Extension;
}

namespace gfx {
class Image;
}

class ExtensionUninstallDialog
    : public content::NotificationObserver,
      public base::SupportsWeakPtr<ExtensionUninstallDialog> {
 public:
  class Delegate {
   public:
    
    virtual void ExtensionUninstallAccepted() = 0;

    
    virtual void ExtensionUninstallCanceled() = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  
  
  static ExtensionUninstallDialog* Create(Profile* profile,
                                          Browser* browser,
                                          Delegate* delegate);

  virtual ~ExtensionUninstallDialog();

  
  
  
  
  void ConfirmUninstall(const extensions::Extension* extension);

 protected:
  
  ExtensionUninstallDialog(Profile* profile,
                           Browser* browser,
                           Delegate* delegate);

  Profile* const profile_;

  Browser* browser_;

  
  Delegate* delegate_;

  
  const extensions::Extension* extension_;

  
  gfx::ImageSkia icon_;

 private:
  
  
  void SetIcon(const gfx::Image& image);

  void OnImageLoaded(const gfx::Image& image);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  virtual void Show() = 0;

  
  
  enum State {
    kImageIsLoading,  
    kDialogIsShowing, 
    kBrowserIsClosing 
  };
  State state_;

  base::MessageLoop* ui_loop_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionUninstallDialog);
};

#endif  
