// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_IMPORTER_HOST_H_
#define CHROME_BROWSER_IMPORTER_IMPORTER_HOST_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "chrome/browser/importer/importer_data_types.h"
#include "chrome/browser/importer/profile_writer.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/native_widget_types.h"

class FirefoxProfileLock;
class Importer;
class Profile;
class Task;

namespace importer {
class ImporterProgressObserver;
}

class ImporterHost : public base::RefCountedThreadSafe<ImporterHost>,
                     public BaseBookmarkModelObserver,
                     public NotificationObserver {
 public:
  ImporterHost();

  
  
  void ShowWarningDialog();

  
  
  
  void OnImportLockDialogEnd(bool is_continue);

  void SetObserver(importer::ImporterProgressObserver* observer);

  
  
  
  void NotifyImportStarted();
  void NotifyImportItemStarted(importer::ImportItem item);
  void NotifyImportItemEnded(importer::ImportItem item);
  void NotifyImportEnded();

  
  
  void set_headless() { headless_ = true; }
  bool is_headless() const { return headless_; }

  void set_parent_window(gfx::NativeWindow parent_window) {
    parent_window_ = parent_window;
  }

  
  
  
  
  
  
  
  virtual void StartImportSettings(
      const importer::SourceProfile& source_profile,
      Profile* target_profile,
      uint16 items,
      ProfileWriter* writer,
      bool first_run);

  
  virtual void Cancel();

 protected:
  ~ImporterHost();

  
  bool ShouldImportToBookmarkBar(bool first_run);

  
  
  
  
  
  
  void CheckForFirefoxLock(const importer::SourceProfile& source_profile,
                           uint16 items,
                           bool first_run);

  
  
  
  void CheckForLoadedModels(uint16 items);

  
  Profile* profile_;

  
  
  
  
  Task* task_;

  
  Importer* importer_;

  
  bool waiting_for_bookmarkbar_model_;

  
  bool installed_bookmark_observer_;

  
  bool is_source_readable_;

  
  NotificationRegistrar registrar_;

  
  scoped_refptr<ProfileWriter> writer_;

 private:
  friend class base::RefCountedThreadSafe<ImporterHost>;

  
  
  
  
  virtual void InvokeTaskIfDone();

  
  virtual void Loaded(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkModelChanged() OVERRIDE;

  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  bool headless_;

  
  
  gfx::NativeWindow parent_window_;

  
  importer::ImporterProgressObserver* observer_;

  
  scoped_ptr<FirefoxProfileLock> firefox_lock_;

  DISALLOW_COPY_AND_ASSIGN(ImporterHost);
};

#endif  
