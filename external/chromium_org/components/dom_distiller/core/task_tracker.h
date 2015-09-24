// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DOM_DISTILLER_CORE_TASK_TRACKER_H_
#define COMPONENTS_DOM_DISTILLER_CORE_TASK_TRACKER_H_

#include <string>
#include <vector>

#include "base/bind.h"
#include "base/memory/weak_ptr.h"
#include "components/dom_distiller/core/article_entry.h"
#include "components/dom_distiller/core/distiller.h"
#include "components/dom_distiller/core/proto/distilled_page.pb.h"

class GURL;

namespace dom_distiller {

class DistilledPageProto;

class ViewerHandle {
  typedef base::Callback<void()> CancelCallback;

 public:
  explicit ViewerHandle(CancelCallback callback);
  ~ViewerHandle();

 private:
  CancelCallback cancel_callback_;
  DISALLOW_COPY_AND_ASSIGN(ViewerHandle);
};

class ViewRequestDelegate {
 public:
  virtual ~ViewRequestDelegate() {}
  
  
  
  
  virtual void OnArticleReady(DistilledPageProto* proto) = 0;
};

class TaskTracker {
 public:
  typedef base::Callback<void(TaskTracker*)> CancelCallback;
  typedef base::Callback<void(const ArticleEntry&, DistilledPageProto*)>
      SaveCallback;

  TaskTracker(const ArticleEntry& entry, CancelCallback callback);
  ~TaskTracker();

  
  void StartDistiller(DistillerFactory* factory);
  void StartBlobFetcher();

  void SetSaveCallback(SaveCallback callback);

  
  scoped_ptr<ViewerHandle> AddViewer(ViewRequestDelegate* delegate);

  bool HasEntryId(const std::string& entry_id) const;
  bool HasUrl(const GURL& url) const;

 private:
  void OnDistilledDataReady(scoped_ptr<DistilledPageProto> distilled_page);
  void DoSaveCallback();

  void RemoveViewer(ViewRequestDelegate* delegate);
  void NotifyViewer(ViewRequestDelegate* delegate);

  void MaybeCancel();

  CancelCallback cancel_callback_;
  SaveCallback save_callback_;

  scoped_ptr<Distiller> distiller_;

  
  
  std::vector<ViewRequestDelegate*> viewers_;

  ArticleEntry entry_;
  scoped_ptr<DistilledPageProto> distilled_page_;

  
  
  base::WeakPtrFactory<TaskTracker> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(TaskTracker);
};

}  

#endif  
