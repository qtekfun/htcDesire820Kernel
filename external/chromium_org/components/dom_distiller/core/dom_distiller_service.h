// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DOM_DISTILLER_CORE_DOM_DISTILLER_SERVICE_H_
#define COMPONENTS_DOM_DISTILLER_CORE_DOM_DISTILLER_SERVICE_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "components/dom_distiller/core/article_entry.h"

class GURL;

namespace syncer {
class SyncableService;
}

namespace dom_distiller {

class DistilledPageProto;
class DistillerFactory;
class DomDistillerObserver;
class DomDistillerStoreInterface;
class TaskTracker;
class ViewerHandle;
class ViewRequestDelegate;

class DomDistillerService {
 public:
  DomDistillerService(scoped_ptr<DomDistillerStoreInterface> store,
                      scoped_ptr<DistillerFactory> distiller_factory);
  ~DomDistillerService();

  syncer::SyncableService* GetSyncableService() const;

  
  
  void AddToList(const GURL& url);

  
  std::vector<ArticleEntry> GetEntries() const;

  
  void RemoveEntry(const std::string& entry_id);

  
  
  
  
  scoped_ptr<ViewerHandle> ViewEntry(ViewRequestDelegate* delegate,
                                     const std::string& entry_id);

  
  scoped_ptr<ViewerHandle> ViewUrl(ViewRequestDelegate* delegate,
                                   const GURL& url);

  void AddObserver(DomDistillerObserver* observer);
  void RemoveObserver(DomDistillerObserver* observer);

 private:
  void CancelTask(TaskTracker* task);
  void AddDistilledPageToList(const ArticleEntry& entry,
                              DistilledPageProto* proto);

  TaskTracker* CreateTaskTracker(const ArticleEntry& entry);

  
  
  
  TaskTracker* GetTaskTrackerForUrl(const GURL& url);
  TaskTracker* GetTaskTrackerForEntry(const ArticleEntry& entry);

  scoped_ptr<DomDistillerStoreInterface> store_;
  scoped_ptr<DistillerFactory> distiller_factory_;

  typedef ScopedVector<TaskTracker> TaskList;
  TaskList tasks_;

  DISALLOW_COPY_AND_ASSIGN(DomDistillerService);
};

}  

#endif  
