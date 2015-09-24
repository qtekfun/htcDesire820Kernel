// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_HOST_H_
#define CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_HOST_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "chrome/browser/importer/importer_progress_observer.h"
#include "chrome/browser/importer/profile_writer.h"
#include "chrome/browser/search_engines/template_url_service.h"
#include "chrome/common/importer/importer_data_types.h"
#include "ui/gfx/native_widget_types.h"

class ExternalProcessImporterClient;
class FirefoxProfileLock;
class Importer;
class Profile;

namespace importer {
struct SourceProfile;
}

class ExternalProcessImporterHost : public BaseBookmarkModelObserver {
 public:
  ExternalProcessImporterHost();

  void Cancel();

  
  
  
  
  
  
  virtual void StartImportSettings(
      const importer::SourceProfile& source_profile,
      Profile* target_profile,
      uint16 items,
      ProfileWriter* writer);

  
  
  
  
  void set_headless() { headless_ = true; }
  bool is_headless() const { return headless_; }

  void set_parent_window(gfx::NativeWindow parent_window) {
    parent_window_ = parent_window;
  }

  void set_observer(importer::ImporterProgressObserver* observer) {
    observer_ = observer;
  }

  
  
  
  void NotifyImportStarted();
  void NotifyImportItemStarted(importer::ImportItem item);
  void NotifyImportItemEnded(importer::ImportItem item);
  void NotifyImportEnded();

 private:
  
  virtual ~ExternalProcessImporterHost();

  
  
  
  
  virtual void LaunchImportIfReady();

  
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE;
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkModelChanged() OVERRIDE;

  
  void OnTemplateURLServiceLoaded();

  
  
  
  void ShowWarningDialog();

  
  
  
  void OnImportLockDialogEnd(bool is_continue);

  
  
  
  
  
  bool CheckForFirefoxLock(const importer::SourceProfile& source_profile);

  
  
  
  void CheckForLoadedModels(uint16 items);

  
  bool headless_;

  
  
  gfx::NativeWindow parent_window_;

  
  importer::ImporterProgressObserver* observer_;

  
  scoped_ptr<FirefoxProfileLock> firefox_lock_;

  
  Profile* profile_;

  
  bool waiting_for_bookmarkbar_model_;

  
  
  scoped_ptr<TemplateURLService::Subscription> template_service_subscription_;

  
  bool installed_bookmark_observer_;

  
  bool is_source_readable_;

  
  scoped_refptr<ProfileWriter> writer_;

  
  ExternalProcessImporterClient* client_;

  
  importer::SourceProfile source_profile_;

  
  uint16 items_;

  
  bool cancelled_;

  
  base::WeakPtrFactory<ExternalProcessImporterHost> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ExternalProcessImporterHost);
};

#endif  
