// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_UNINSTALL_DIALOG_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_UNINSTALL_DIALOG_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "third_party/skia/include/core/SkBitmap.h"

class MessageLoop;
class Profile;

class ExtensionUninstallDialog : public ImageLoadingTracker::Observer {
 public:
  class Delegate {
   public:
    
    virtual void ExtensionDialogAccepted() = 0;

    
    virtual void ExtensionDialogCanceled() = 0;

   protected:
    virtual ~Delegate() {}
  };

  explicit ExtensionUninstallDialog(Profile* profile);
  virtual ~ExtensionUninstallDialog();

  
  
  
  
  
  void ConfirmUninstall(Delegate* delegate, const Extension* extension);

 private:
  
  static void Show(Profile* profile,
                   Delegate* delegate,
                   const Extension* extension,
                   SkBitmap* icon);

  
  
  void SetIcon(SkBitmap* icon);

  
  virtual void OnImageLoaded(SkBitmap* image,
                             const ExtensionResource& resource,
                             int index) OVERRIDE;

  Profile* profile_;
  MessageLoop* ui_loop_;

  
  Delegate* delegate_;

  
  const Extension* extension_;

  
  
  ImageLoadingTracker tracker_;

  
  SkBitmap icon_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionUninstallDialog);
};

#endif  
