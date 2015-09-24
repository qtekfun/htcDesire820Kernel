// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_DIALOG_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_DIALOG_H_

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/views/window/dialog_delegate.h"

class ExtensionDialogObserver;
class GURL;
class Profile;

namespace content {
class WebContents;
}

namespace extensions {
class ExtensionViewHost;
}

namespace ui {
class BaseWindow;
}

class ExtensionDialog : public views::DialogDelegate,
                        public content::NotificationObserver,
                        public base::RefCounted<ExtensionDialog> {
 public:
  
  
  
  
  
  static ExtensionDialog* Show(const GURL& url,
                               ui::BaseWindow* base_window,
                               Profile* profile,
                               content::WebContents* web_contents,
                               int width,
                               int height,
                               int min_width,
                               int min_height,
                               const base::string16& title,
                               ExtensionDialogObserver* observer);

  
  
  void ObserverDestroyed();

  
  void MaybeFocusRenderView();

  
  void set_title(const base::string16& title) { window_title_ = title; }

  
  void SetMinimumContentsSize(int width, int height);

  extensions::ExtensionViewHost* host() const { return host_.get(); }

  
  virtual int GetDialogButtons() const OVERRIDE;
  virtual bool CanResize() const OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;
  virtual bool ShouldShowWindowTitle() const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual void WindowClosing() OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual views::Widget* GetWidget() OVERRIDE;
  virtual const views::Widget* GetWidget() const OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;
  virtual bool UseNewStyleForThisDialog() const OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 protected:
  virtual ~ExtensionDialog();

 private:
  friend class base::RefCounted<ExtensionDialog>;

  
  ExtensionDialog(extensions::ExtensionViewHost* host,
                  ExtensionDialogObserver* observer);

  void InitWindow(ui::BaseWindow* base_window, int width, int height);

  
  base::string16 window_title_;

  
  scoped_ptr<extensions::ExtensionViewHost> host_;

  content::NotificationRegistrar registrar_;

  
  ExtensionDialogObserver* observer_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionDialog);
};

#endif  
