// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_DATA_MEMOIZING_STORE_H_
#define CONTENT_BROWSER_RENDERER_DATA_MEMOIZING_STORE_H_

#include <map>

#include "base/bind.h"
#include "base/synchronization/lock.h"
#include "content/browser/renderer_host/render_view_host_impl.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_process_host_observer.h"

namespace content {

template <typename T>
class RendererDataMemoizingStore : public RenderProcessHostObserver {
 public:
  RendererDataMemoizingStore() : next_item_id_(1) {
  }

  ~RendererDataMemoizingStore() {
    DCHECK_EQ(0U, id_to_item_.size()) << "Failed to outlive render processes";
  }

  
  
  
  int Store(T* item, int process_id) {
    DCHECK(item);
    base::AutoLock auto_lock(lock_);

    int item_id;

    
    typename ReverseItemMap::iterator item_iter = item_to_id_.find(item);
    if (item_iter == item_to_id_.end()) {
      item_id = next_item_id_++;
      
      
      if (next_item_id_ == 0)
        next_item_id_ = 1;
      id_to_item_[item_id] = item;
      item_to_id_[item] = item_id;
    } else {
      item_id = item_iter->second;
    }

    
    std::pair<IDMap::iterator, IDMap::iterator> process_ids =
        process_id_to_item_id_.equal_range(process_id);
    bool already_watching_process = (process_ids.first != process_ids.second);
    if (std::find_if(process_ids.first, process_ids.second,
                     MatchSecond<int>(item_id)) == process_ids.second) {
      process_id_to_item_id_.insert(std::make_pair(process_id, item_id));
    }

    
    std::pair<IDMap::iterator, IDMap::iterator> item_ids =
        item_id_to_process_id_.equal_range(item_id);
    if (std::find_if(item_ids.first, item_ids.second,
                     MatchSecond<int>(process_id)) == item_ids.second) {
      item_id_to_process_id_.insert(std::make_pair(item_id, process_id));
    }

    
    if (!already_watching_process) {
      if (BrowserThread::CurrentlyOn(BrowserThread::UI)) {
        StartObservingProcess(process_id);
      } else {
        BrowserThread::PostTask(
            BrowserThread::UI,
            FROM_HERE,
            base::Bind(&RendererDataMemoizingStore::StartObservingProcess,
                       base::Unretained(this),
                       process_id));
      }
    }

    return item_id;
  }

  
  
  
  bool Retrieve(int item_id, scoped_refptr<T>* item) {
    base::AutoLock auto_lock(lock_);

    typename ItemMap::iterator iter = id_to_item_.find(item_id);
    if (iter == id_to_item_.end())
      return false;
    if (item)
      *item = iter->second;
    return true;
  }

 private:
  typedef std::multimap<int, int> IDMap;
  typedef std::map<int, scoped_refptr<T> > ItemMap;
  typedef std::map<T*, int, typename T::LessThan> ReverseItemMap;

  template <typename M>
  struct MatchSecond {
    explicit MatchSecond(const M& t) : value(t) {}

    template <typename Pair>
    bool operator()(const Pair& p) const {
      return (value == p.second);
    }

    M value;
  };

  void StartObservingProcess(int process_id) {
    DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
    RenderProcessHost* host = RenderProcessHost::FromID(process_id);
    if (!host) {
      
      
      
      RemoveRenderProcessItems(process_id);
      return;
    }

    host->AddObserver(this);
  }

  
  
  void RemoveInternal(int item_id) {
    typename ItemMap::iterator item_iter = id_to_item_.find(item_id);
    DCHECK(item_iter != id_to_item_.end());

    typename ReverseItemMap::iterator id_iter =
        item_to_id_.find(item_iter->second.get());
    DCHECK(id_iter != item_to_id_.end());
    item_to_id_.erase(id_iter);

    id_to_item_.erase(item_iter);
  }

  void RenderProcessHostDestroyed(RenderProcessHost* host) {
    DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
    RemoveRenderProcessItems(host->GetID());
  }

  
  void RemoveRenderProcessItems(int process_id) {
    base::AutoLock auto_lock(lock_);

    
    std::pair<IDMap::iterator, IDMap::iterator> process_ids =
        process_id_to_item_id_.equal_range(process_id);
    for (IDMap::iterator ids_iter = process_ids.first;
         ids_iter != process_ids.second; ++ids_iter) {
      int item_id = ids_iter->second;
      
      
      
      std::pair<IDMap::iterator, IDMap::iterator> item_ids =
          item_id_to_process_id_.equal_range(item_id);
      IDMap::iterator proc_iter = std::find_if(
          item_ids.first, item_ids.second, MatchSecond<int>(process_id));
      DCHECK(proc_iter != item_ids.second);

      
      
      
      
      
      IDMap::iterator next_proc_iter = proc_iter;
      ++next_proc_iter;
      bool last_process_for_item_id =
          (proc_iter == item_ids.first && next_proc_iter == item_ids.second);
      item_id_to_process_id_.erase(proc_iter);

      if (last_process_for_item_id) {
        
        
        RemoveInternal(item_id);
      }
    }
    if (process_ids.first != process_ids.second)
      process_id_to_item_id_.erase(process_ids.first, process_ids.second);
  }

  IDMap process_id_to_item_id_;
  IDMap item_id_to_process_id_;
  ItemMap id_to_item_;
  ReverseItemMap item_to_id_;

  int next_item_id_;

  
  
  base::Lock lock_;
};

}  

#endif  
