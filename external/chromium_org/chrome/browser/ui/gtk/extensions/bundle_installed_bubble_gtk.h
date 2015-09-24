// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_EXTENSIONS_BUNDLE_INSTALLED_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_EXTENSIONS_BUNDLE_INSTALLED_BUBBLE_GTK_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/bundle_installer.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"
#include "chrome/browser/ui/gtk/custom_button.h"

class Browser;

class BundleInstalledBubbleGtk
    : public BubbleDelegateGtk,
      public base::RefCounted<BundleInstalledBubbleGtk> {
 public:
  
  BundleInstalledBubbleGtk(const extensions::BundleInstaller* bundle,
                           Browser* browser);

 private:
  friend class base::RefCounted<BundleInstalledBubbleGtk>;

  virtual ~BundleInstalledBubbleGtk();

  
  void ShowInternal(const extensions::BundleInstaller* bundle);

  
  
  void InsertExtensionList(GtkWidget* parent,
                           const extensions::BundleInstaller* bundle,
                           extensions::BundleInstaller::Item::State state);

  
  virtual void BubbleClosing(BubbleGtk* bubble, bool closed_by_escape) OVERRIDE;

  
  void Close();

  
  static void OnButtonClick(GtkWidget* button,
                            BundleInstalledBubbleGtk* bubble);

  Browser* browser_;
  scoped_ptr<CustomDrawButton> close_button_;
  BubbleGtk* bubble_;

  DISALLOW_COPY_AND_ASSIGN(BundleInstalledBubbleGtk);
};

#endif  
