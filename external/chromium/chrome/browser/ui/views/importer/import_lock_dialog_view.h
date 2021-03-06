// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_IMPORTER_IMPORT_LOCK_DIALOG_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_IMPORTER_IMPORT_LOCK_DIALOG_VIEW_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "views/view.h"
#include "views/window/dialog_delegate.h"

namespace views {
class Label;
}

class ImporterHost;

class ImportLockDialogView : public views::View,
                             public views::DialogDelegate {
 public:
  static void Show(gfx::NativeWindow parent, ImporterHost* importer_host);

 private:
  explicit ImportLockDialogView(ImporterHost* importer_host);
  virtual ~ImportLockDialogView();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;

  
  virtual std::wstring GetDialogButtonLabel(
      MessageBoxFlags::DialogButton button) const OVERRIDE;
  virtual bool IsModal() const OVERRIDE;
  virtual std::wstring GetWindowTitle() const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;

 private:
  views::Label* description_label_;

  
  scoped_refptr<ImporterHost> importer_host_;

  DISALLOW_COPY_AND_ASSIGN(ImportLockDialogView);
};

#endif  
