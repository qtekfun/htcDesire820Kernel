// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_SELECT_FILE_DIALOG_EXTENSION_H_
#define CHROME_BROWSER_UI_VIEWS_SELECT_FILE_DIALOG_EXTENSION_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/ui/views/extensions/extension_dialog_observer.h"
#include "ui/gfx/native_widget_types.h"  
#include "ui/shell_dialogs/select_file_dialog.h"

class ExtensionDialog;
class Profile;

namespace content {
class RenderViewHost;
class WebContents;
}

namespace ui {
struct SelectedFileInfo;
class SelectFilePolicy;
}

class SelectFileDialogExtension
    : public ui::SelectFileDialog,
      public ExtensionDialogObserver {
 public:
  
  
  typedef const void* RoutingID;
  static RoutingID GetRoutingIDFromWebContents(
      const content::WebContents* web_contents);

  static SelectFileDialogExtension* Create(
      ui::SelectFileDialog::Listener* listener,
      ui::SelectFilePolicy* policy);

  
  virtual bool IsRunning(gfx::NativeWindow owner_window) const OVERRIDE;
  virtual void ListenerDestroyed() OVERRIDE;

  
  virtual void ExtensionDialogClosing(ExtensionDialog* dialog) OVERRIDE;
  virtual void ExtensionTerminated(ExtensionDialog* dialog) OVERRIDE;

  
  
  static void OnFileSelected(RoutingID routing_id,
                             const ui::SelectedFileInfo& file,
                             int index);
  static void OnMultiFilesSelected(
      RoutingID routing_id,
      const std::vector<ui::SelectedFileInfo>& files);
  static void OnFileSelectionCanceled(RoutingID routing_id);

  
  content::RenderViewHost* GetRenderViewHost();

 protected:
  
  virtual void SelectFileImpl(
      Type type,
      const base::string16& title,
      const base::FilePath& default_path,
      const FileTypeInfo* file_types,
      int file_type_index,
      const base::FilePath::StringType& default_extension,
      gfx::NativeWindow owning_window,
      void* params) OVERRIDE;

 private:
  friend class SelectFileDialogExtensionBrowserTest;
  friend class SelectFileDialogExtensionTest;

  
  explicit SelectFileDialogExtension(SelectFileDialog::Listener* listener,
                                     ui::SelectFilePolicy* policy);
  virtual ~SelectFileDialogExtension();

  
  void NotifyListener();

  
  void AddPending(RoutingID routing_id);

  
  static bool PendingExists(RoutingID routing_id);

  
  virtual bool HasMultipleFileTypeChoicesImpl() OVERRIDE;

  bool has_multiple_file_type_choices_;

  
  scoped_refptr<ExtensionDialog> extension_dialog_;

  
  RoutingID routing_id_;

  
  Profile* profile_;

  
  gfx::NativeWindow owner_window_;

  
  
  
  enum SelectionType {
    CANCEL = 0,
    SINGLE_FILE,
    MULTIPLE_FILES
  };
  SelectionType selection_type_;
  std::vector<ui::SelectedFileInfo> selection_files_;
  int selection_index_;
  void* params_;

  DISALLOW_COPY_AND_ASSIGN(SelectFileDialogExtension);
};

#endif  
