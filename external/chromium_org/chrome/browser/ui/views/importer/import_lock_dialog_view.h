// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_IMPORTER_IMPORT_LOCK_DIALOG_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_IMPORTER_IMPORT_LOCK_DIALOG_VIEW_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "ui/views/view.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {
class Label;
}

class ImportLockDialogView : public views::DialogDelegateView {
 public:
  static void Show(gfx::NativeWindow parent,
                   const base::Callback<void(bool)>& callback);

 private:
  explicit ImportLockDialogView(const base::Callback<void(bool)>& callback);
  virtual ~ImportLockDialogView();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;

  
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;

 private:
  views::Label* description_label_;

  
  base::Callback<void(bool)> callback_;

  DISALLOW_COPY_AND_ASSIGN(ImportLockDialogView);
};

#endif  
