// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_CREATE_APPLICATION_SHORTCUTS_DIALOG_GTK_H_
#define CHROME_BROWSER_UI_GTK_CREATE_APPLICATION_SHORTCUTS_DIALOG_GTK_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "chrome/browser/shell_integration.h"
#include "content/browser/browser_thread.h"
#include "googleurl/src/gurl.h"
#include "ui/base/gtk/gtk_signal.h"

typedef struct _GdkPixbuf GdkPixbuf;
typedef struct _GtkWidget GtkWidget;
typedef struct _GtkWindow GtkWindow;

class Extension;
class TabContents;
class TabContentsWrapper;

class CreateApplicationShortcutsDialogGtk
    : public base::RefCountedThreadSafe<CreateApplicationShortcutsDialogGtk,
                                        BrowserThread::DeleteOnUIThread> {
 protected:
  explicit CreateApplicationShortcutsDialogGtk(GtkWindow* parent);
  virtual ~CreateApplicationShortcutsDialogGtk();

  CHROMEGTK_CALLBACK_1(CreateApplicationShortcutsDialogGtk, void,
                       OnCreateDialogResponse, int);

  CHROMEGTK_CALLBACK_1(CreateApplicationShortcutsDialogGtk, void,
                       OnErrorDialogResponse, int);

  CHROMEGTK_CALLBACK_0(CreateApplicationShortcutsDialogGtk, void,
                       OnToggleCheckbox);

  virtual void CreateDialogBox(GtkWindow* parent);
  virtual void CreateIconPixBuf(const SkBitmap& bitmap);

  
  
  virtual void OnCreatedShortcut(void) {}

  void CreateDesktopShortcut(
      const ShellIntegration::ShortcutInfo& shortcut_info);
  void ShowErrorDialog();

  GtkWindow* parent_;

  
  GtkWidget* desktop_checkbox_;
  GtkWidget* menu_checkbox_;

  
  ShellIntegration::ShortcutInfo shortcut_info_;

  
  GdkPixbuf* favicon_pixbuf_;

  
  GtkWidget* create_dialog_;

  
  GtkWidget* error_dialog_;

 private:
  friend struct BrowserThread::DeleteOnThread<BrowserThread::UI>;
  friend class DeleteTask<CreateApplicationShortcutsDialogGtk>;
  DISALLOW_COPY_AND_ASSIGN(CreateApplicationShortcutsDialogGtk);
};

class CreateWebApplicationShortcutsDialogGtk
    : public CreateApplicationShortcutsDialogGtk {
 public:
  
  static void Show(GtkWindow* parent, TabContentsWrapper* tab_contents);

  CreateWebApplicationShortcutsDialogGtk(GtkWindow* parent,
                                         TabContentsWrapper* tab_contents);
  virtual ~CreateWebApplicationShortcutsDialogGtk() {}

  virtual void OnCreatedShortcut(void);

 private:

  
  TabContentsWrapper* tab_contents_;

  DISALLOW_COPY_AND_ASSIGN(CreateWebApplicationShortcutsDialogGtk);
};

class CreateChromeApplicationShortcutsDialogGtk
  : public CreateApplicationShortcutsDialogGtk,
    public ImageLoadingTracker::Observer {
 public:
  
  static void Show(GtkWindow* parent, const Extension* app);

  explicit CreateChromeApplicationShortcutsDialogGtk(GtkWindow* parent,
                                                     const Extension* app);
  virtual ~CreateChromeApplicationShortcutsDialogGtk() {}

  
  
  
  virtual void OnImageLoaded(SkBitmap* image,
                             const ExtensionResource& resource,
                             int index);

 private:
  const Extension* app_;
  ImageLoadingTracker tracker_;
  DISALLOW_COPY_AND_ASSIGN(CreateChromeApplicationShortcutsDialogGtk);
};

#endif  
