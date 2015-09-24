// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_BOOKMARK_DATA_TYPE_CONTROLLER_H__
#define CHROME_BROWSER_SYNC_GLUE_BOOKMARK_DATA_TYPE_CONTROLLER_H__

#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "chrome/browser/sync/glue/frontend_data_type_controller.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace browser_sync {

class BookmarkDataTypeController : public FrontendDataTypeController,
                                   public content::NotificationObserver,
                                   public BaseBookmarkModelObserver {
 public:
  BookmarkDataTypeController(ProfileSyncComponentsFactory* profile_sync_factory,
                             Profile* profile,
                             ProfileSyncService* sync_service);

  
  virtual syncer::ModelType type() const OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  virtual ~BookmarkDataTypeController();

  
  virtual bool StartModels() OVERRIDE;
  virtual void CleanUpState() OVERRIDE;
  virtual void CreateSyncComponents() OVERRIDE;

  
  virtual void BookmarkModelChanged() OVERRIDE;
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE;
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model) OVERRIDE;

  
  
  bool DependentsLoaded();

  content::NotificationRegistrar registrar_;

  BookmarkModel* bookmark_model_;

  
  bool installed_bookmark_observer_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkDataTypeController);
};

}  

#endif  
