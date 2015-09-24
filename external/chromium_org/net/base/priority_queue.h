// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_PRIORITY_QUEUE_H_
#define NET_BASE_PRIORITY_QUEUE_H_

#include <list>
#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/net_export.h"

#if !defined(NDEBUG)
#include "base/containers/hash_tables.h"
#endif

namespace net {

template<typename T>
class PriorityQueue : public base::NonThreadSafe {
 private:
  
#if !defined(NDEBUG)
  typedef std::list<std::pair<unsigned, T> > List;
#else
  typedef std::list<T> List;
#endif

 public:
  typedef uint32 Priority;

  
  
  class Pointer {
   public:
    
    Pointer() : priority_(kNullPriority) {
#if !defined(NDEBUG)
      id_ = static_cast<unsigned>(-1);
#endif
      
      
      
      
      iterator_ = dummy_empty_list_.end();
    }

    Pointer(const Pointer& p)
        : priority_(p.priority_),
          iterator_(p.iterator_) {
#if !defined(NDEBUG)
      id_ = p.id_;
#endif
    }

    Pointer& operator=(const Pointer& p) {
      
      priority_ = p.priority_;
      iterator_ = p.iterator_;
#if !defined(NDEBUG)
      id_ = p.id_;
#endif
      return *this;
    }

    bool is_null() const { return priority_ == kNullPriority; }

    Priority priority() const { return priority_; }

#if !defined(NDEBUG)
    const T& value() const { return iterator_->second; }
#else
    const T& value() const { return *iterator_; }
#endif

    
    bool Equals(const Pointer& other) const {
      return (priority_ == other.priority_) && (iterator_ == other.iterator_);
    }

    void Reset() {
      *this = Pointer();
    }

   private:
    friend class PriorityQueue;

    
    
    
    
    typedef typename PriorityQueue::List::iterator ListIterator;

    static const Priority kNullPriority = static_cast<Priority>(-1);

    
    
    Pointer(Priority priority, const ListIterator& iterator)
        : priority_(priority), iterator_(iterator) {
#if !defined(NDEBUG)
      id_ = iterator_->first;
#endif
    }

    Priority priority_;
    ListIterator iterator_;
    
    
    
    List dummy_empty_list_;

#if !defined(NDEBUG)
    
    unsigned id_;
#endif
  };

  
  explicit PriorityQueue(Priority num_priorities)
      : lists_(num_priorities), size_(0) {
#if !defined(NDEBUG)
    next_id_ = 0;
#endif
  }

  
  
  Pointer Insert(const T& value, Priority priority) {
    DCHECK(CalledOnValidThread());
    DCHECK_LT(priority, lists_.size());
    ++size_;
    List& list = lists_[priority];
#if !defined(NDEBUG)
    unsigned id = next_id_;
    valid_ids_.insert(id);
    ++next_id_;
    return Pointer(priority, list.insert(list.end(),
                                         std::make_pair(id, value)));
#else
    return Pointer(priority, list.insert(list.end(), value));
#endif
  }

  
  
  Pointer InsertAtFront(const T& value, Priority priority) {
    DCHECK(CalledOnValidThread());
    DCHECK_LT(priority, lists_.size());
    ++size_;
    List& list = lists_[priority];
#if !defined(NDEBUG)
    unsigned id = next_id_;
    valid_ids_.insert(id);
    ++next_id_;
    return Pointer(priority, list.insert(list.begin(),
                                         std::make_pair(id, value)));
#else
    return Pointer(priority, list.insert(list.begin(), value));
#endif
  }

  
  
  void Erase(const Pointer& pointer) {
    DCHECK(CalledOnValidThread());
    DCHECK_LT(pointer.priority_, lists_.size());
    DCHECK_GT(size_, 0u);

#if !defined(NDEBUG)
    DCHECK_EQ(1u, valid_ids_.erase(pointer.id_));
    DCHECK_EQ(pointer.iterator_->first, pointer.id_);
#endif

    --size_;
    lists_[pointer.priority_].erase(pointer.iterator_);
  }

  
  
  Pointer FirstMin() const {
    DCHECK(CalledOnValidThread());
    for (size_t i = 0; i < lists_.size(); ++i) {
      List* list = const_cast<List*>(&lists_[i]);
      if (!list->empty())
        return Pointer(i, list->begin());
    }
    return Pointer();
  }

  
  
  Pointer LastMin() const {
    DCHECK(CalledOnValidThread());
    for (size_t i = 0; i < lists_.size(); ++i) {
      List* list = const_cast<List*>(&lists_[i]);
      if (!list->empty())
        return Pointer(i, --list->end());
    }
    return Pointer();
  }

  
  
  Pointer FirstMax() const {
    DCHECK(CalledOnValidThread());
    for (size_t i = lists_.size(); i > 0; --i) {
      size_t index = i - 1;
      List* list = const_cast<List*>(&lists_[index]);
      if (!list->empty())
        return Pointer(index, list->begin());
    }
    return Pointer();
  }

  
  
  Pointer LastMax() const {
    DCHECK(CalledOnValidThread());
    for (size_t i = lists_.size(); i > 0; --i) {
      size_t index = i - 1;
      List* list = const_cast<List*>(&lists_[index]);
      if (!list->empty())
        return Pointer(index, --list->end());
    }
    return Pointer();
  }

  
  
  
  
  
  
  
  
  Pointer GetNextTowardsLastMin(const Pointer& pointer) const {
    DCHECK(CalledOnValidThread());
    DCHECK(!pointer.is_null());
    DCHECK_LT(pointer.priority_, lists_.size());

    typename Pointer::ListIterator it = pointer.iterator_;
    Priority priority = pointer.priority_;
    DCHECK(it != lists_[priority].end());
    ++it;
    while (it == lists_[priority].end()) {
      if (priority == 0u)
        return Pointer();
      --priority;
      it = const_cast<List*>(&lists_[priority])->begin();
    }
    return Pointer(priority, it);
  }

  
  void Clear() {
    DCHECK(CalledOnValidThread());
    for (size_t i = 0; i < lists_.size(); ++i) {
      lists_[i].clear();
    }
#if !defined(NDEBUG)
    valid_ids_.clear();
#endif
    size_ = 0u;
  }

  
  size_t num_priorities() const {
    DCHECK(CalledOnValidThread());
    return lists_.size();
  }

  bool empty() const {
    DCHECK(CalledOnValidThread());
    return size_ == 0;
  }

  
  size_t size() const {
    DCHECK(CalledOnValidThread());
    return size_;
  }

 private:
  typedef std::vector<List> ListVector;

#if !defined(NDEBUG)
  unsigned next_id_;
  base::hash_set<unsigned> valid_ids_;
#endif

  ListVector lists_;
  size_t size_;

  DISALLOW_COPY_AND_ASSIGN(PriorityQueue);
};

}  

#endif  
