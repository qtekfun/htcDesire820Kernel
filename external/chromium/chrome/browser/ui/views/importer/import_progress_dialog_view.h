// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_IMPORTER_IMPORT_PROGRESS_DIALOG_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_IMPORTER_IMPORT_PROGRESS_DIALOG_VIEW_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/importer/importer_data_types.h"
#include "chrome/browser/importer/importer_progress_observer.h"
#include "views/view.h"
#include "views/window/dialog_delegate.h"

class ImporterHost;
class ImporterObserver;

namespace views {
class CheckmarkThrobber;
class Label;
}

class ImportProgressDialogView : public views::View,
                                 public views::DialogDelegate,
                                 public importer::ImporterProgressObserver {
 public:
  
  
  
  ImportProgressDialogView(HWND parent_window,
                           uint16 items,
                           ImporterHost* importer_host,
                           ImporterObserver* importer_observer,
                           const string16& importer_name,
                           bool bookmarks_import);
  virtual ~ImportProgressDialogView();

 protected:
  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child) OVERRIDE;

  
  virtual int GetDialogButtons() const OVERRIDE;
  virtual std::wstring GetDialogButtonLabel(
      MessageBoxFlags::DialogButton button) const OVERRIDE;
  virtual bool IsModal() const OVERRIDE;
  virtual std::wstring GetWindowTitle() const OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;

 private:
  
  void InitControlLayout();

  
  virtual void ImportStarted() OVERRIDE;
  virtual void ImportItemStarted(importer::ImportItem item) OVERRIDE;
  virtual void ImportItemEnded(importer::ImportItem item) OVERRIDE;
  virtual void ImportEnded() OVERRIDE;

  
  HWND parent_window_;

  
  scoped_ptr<views::CheckmarkThrobber> state_bookmarks_;
  scoped_ptr<views::CheckmarkThrobber> state_searches_;
  scoped_ptr<views::CheckmarkThrobber> state_passwords_;
  scoped_ptr<views::CheckmarkThrobber> state_history_;
  scoped_ptr<views::CheckmarkThrobber> state_cookies_;
  views::Label* label_info_;
  scoped_ptr<views::Label> label_bookmarks_;
  scoped_ptr<views::Label> label_searches_;
  scoped_ptr<views::Label> label_passwords_;
  scoped_ptr<views::Label> label_history_;
  scoped_ptr<views::Label> label_cookies_;

  
  uint16 items_;

  
  scoped_refptr<ImporterHost> importer_host_;

  
  ImporterObserver* importer_observer_;

  
  bool importing_;

  
  bool bookmarks_import_;

  DISALLOW_COPY_AND_ASSIGN(ImportProgressDialogView);
};

#endif  
