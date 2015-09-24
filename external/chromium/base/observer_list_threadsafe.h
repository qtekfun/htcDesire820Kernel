// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_OBSERVER_LIST_THREADSAFE_H_
#define BASE_OBSERVER_LIST_THREADSAFE_H_
#pragma once

#include <algorithm>
#include <map>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop.h"
#include "base/observer_list.h"
#include "base/task.h"


template <class ObserverType>
class ObserverListThreadSafe;

template <class T>
struct ObserverListThreadSafeTraits {
  static void Destruct(const ObserverListThreadSafe<T>* x) {
    delete x;
  }
};

template <class ObserverType>
class ObserverListThreadSafe
    : public base::RefCountedThreadSafe<
        ObserverListThreadSafe<ObserverType>,
        ObserverListThreadSafeTraits<ObserverType> > {
 public:
  typedef typename ObserverList<ObserverType>::NotificationType
      NotificationType;

  ObserverListThreadSafe()
      : type_(ObserverListBase<ObserverType>::NOTIFY_ALL) {}
  explicit ObserverListThreadSafe(NotificationType type) : type_(type) {}

  
  void AddObserver(ObserverType* obs) {
    ObserverList<ObserverType>* list = NULL;
    MessageLoop* loop = MessageLoop::current();
    
    
    
    if (!loop)
      return;  
    {
      base::AutoLock lock(list_lock_);
      if (observer_lists_.find(loop) == observer_lists_.end())
        observer_lists_[loop] = new ObserverList<ObserverType>(type_);
      list = observer_lists_[loop];
    }
    list->AddObserver(obs);
  }

  
  
  
  
  
  void RemoveObserver(ObserverType* obs) {
    ObserverList<ObserverType>* list = NULL;
    MessageLoop* loop = MessageLoop::current();
    if (!loop)
      return;  
    {
      base::AutoLock lock(list_lock_);
      list = observer_lists_[loop];
      if (!list) {
        NOTREACHED() << "RemoveObserver called on for unknown thread";
        return;
      }

      
      
      if (list->size() == 1)
        observer_lists_.erase(loop);
    }
    list->RemoveObserver(obs);

    
    
    
    if (list->size() == 0)
      delete list;
  }

  
  
  
  
  
  template <class Method>
  void Notify(Method m) {
    UnboundMethod<ObserverType, Method, Tuple0> method(m, MakeTuple());
    Notify<Method, Tuple0>(method);
  }

  template <class Method, class A>
  void Notify(Method m, const A &a) {
    UnboundMethod<ObserverType, Method, Tuple1<A> > method(m, MakeTuple(a));
    Notify<Method, Tuple1<A> >(method);
  }

  

 private:
  
  friend struct ObserverListThreadSafeTraits<ObserverType>;

  ~ObserverListThreadSafe() {
    typename ObserversListMap::const_iterator it;
    for (it = observer_lists_.begin(); it != observer_lists_.end(); ++it)
      delete (*it).second;
    observer_lists_.clear();
  }

  template <class Method, class Params>
  void Notify(const UnboundMethod<ObserverType, Method, Params>& method) {
    base::AutoLock lock(list_lock_);
    typename ObserversListMap::iterator it;
    for (it = observer_lists_.begin(); it != observer_lists_.end(); ++it) {
      MessageLoop* loop = (*it).first;
      ObserverList<ObserverType>* list = (*it).second;
      loop->PostTask(
          FROM_HERE,
          NewRunnableMethod(this,
              &ObserverListThreadSafe<ObserverType>::
                 template NotifyWrapper<Method, Params>, list, method));
    }
  }

  
  
  
  template <class Method, class Params>
  void NotifyWrapper(ObserverList<ObserverType>* list,
      const UnboundMethod<ObserverType, Method, Params>& method) {

    
    {
      base::AutoLock lock(list_lock_);
      typename ObserversListMap::iterator it =
          observer_lists_.find(MessageLoop::current());

      
      
      
      
      if (it == observer_lists_.end() || it->second != list)
        return;
    }

    {
      typename ObserverList<ObserverType>::Iterator it(*list);
      ObserverType* obs;
      while ((obs = it.GetNext()) != NULL)
        method.Run(obs);
    }

    
    if (list->size() == 0) {
      {
        base::AutoLock lock(list_lock_);
        
        
        
        typename ObserversListMap::iterator it =
            observer_lists_.find(MessageLoop::current());
        if (it != observer_lists_.end() && it->second == list)
          observer_lists_.erase(it);
      }
      delete list;
    }
  }

  typedef std::map<MessageLoop*, ObserverList<ObserverType>*> ObserversListMap;

  
  base::Lock list_lock_;  
  ObserversListMap observer_lists_;
  const NotificationType type_;

  DISALLOW_COPY_AND_ASSIGN(ObserverListThreadSafe);
};

#endif  
