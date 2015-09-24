// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WORKER_HOST_WORKER_DOCUMENT_SET_H_
#define CONTENT_BROWSER_WORKER_HOST_WORKER_DOCUMENT_SET_H_

#include <set>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

namespace content {
class WorkerMessageFilter;

class WorkerDocumentSet : public base::RefCounted<WorkerDocumentSet> {
 public:
  WorkerDocumentSet();

  
  class DocumentInfo {
   public:
    DocumentInfo(WorkerMessageFilter* filter, unsigned long long document_id,
                 int renderer_process_id, int render_view_id);
    WorkerMessageFilter* filter() const { return filter_; }
    unsigned long long document_id() const { return document_id_; }
    int render_process_id() const { return render_process_id_; }
    int render_view_id() const { return render_view_id_; }

    
    
    bool operator <(const DocumentInfo& other) const {
      
      
      
      if (filter() == other.filter()) {
        return document_id() < other.document_id();
      } else {
        return reinterpret_cast<unsigned long long>(filter()) <
            reinterpret_cast<unsigned long long>(other.filter());
      }
    }

   private:
    WorkerMessageFilter* filter_;
    unsigned long long document_id_;
    int render_process_id_;
    int render_view_id_;
  };

  
  
  
  void Add(WorkerMessageFilter* parent,
           unsigned long long document_id,
           int render_process_id,
           int render_view_id);

  
  bool Contains(WorkerMessageFilter* parent,
                unsigned long long document_id) const;

  
  
  void Remove(WorkerMessageFilter* parent, unsigned long long document_id);

  
  
  void RemoveAll(WorkerMessageFilter* parent);

  bool IsEmpty() const { return document_set_.empty(); }

  
  typedef std::set<DocumentInfo> DocumentInfoSet;

  
  const DocumentInfoSet& documents() { return document_set_; }

 private:
  friend class base::RefCounted<WorkerDocumentSet>;
  virtual ~WorkerDocumentSet();

  DocumentInfoSet document_set_;
};

}  

#endif  
