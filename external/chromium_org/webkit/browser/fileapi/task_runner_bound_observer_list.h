// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_TASK_RUNNER_BOUND_OBSERVER_LIST_H_
#define WEBKIT_BROWSER_FILEAPI_TASK_RUNNER_BOUND_OBSERVER_LIST_H_

#include <map>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/memory/ref_counted.h"
#include "base/sequenced_task_runner.h"
#include "base/threading/thread.h"

namespace fileapi {

template <class T, class Method, class Params>
void NotifyWrapper(T obj, Method m, const Params& p) {
  DispatchToMethod(base::internal::UnwrapTraits<T>::Unwrap(obj), m, p);
}

template <class Observer, class ObserverStoreType = Observer*>
class TaskRunnerBoundObserverList {
 public:
  typedef scoped_refptr<base::SequencedTaskRunner> TaskRunnerPtr;
  typedef std::map<ObserverStoreType, TaskRunnerPtr> ObserversListMap;

  
  TaskRunnerBoundObserverList<Observer, ObserverStoreType>() {}

  
  explicit TaskRunnerBoundObserverList<Observer, ObserverStoreType>(
      const ObserversListMap& observers)
      : observers_(observers) {}

  virtual ~TaskRunnerBoundObserverList<Observer, ObserverStoreType>() {}

  
  
  
  
  
  TaskRunnerBoundObserverList<Observer, ObserverStoreType> AddObserver(
      Observer* observer,
      base::SequencedTaskRunner* runner_to_notify) const {
    ObserversListMap observers = observers_;
    observers.insert(std::make_pair(observer, runner_to_notify));
    return TaskRunnerBoundObserverList<Observer, ObserverStoreType>(observers);
  }

  
  
  template <class Method, class Params>
  void Notify(Method method, const Params& params) const {
    COMPILE_ASSERT(
        (base::internal::ParamsUseScopedRefptrCorrectly<Params>::value),
        badunboundmethodparams);
    for (typename ObserversListMap::const_iterator it = observers_.begin();
         it != observers_.end(); ++it) {
      if (!it->second.get() || it->second->RunsTasksOnCurrentThread()) {
        DispatchToMethod(UnwrapTraits::Unwrap(it->first), method, params);
        continue;
      }
      it->second->PostTask(
          FROM_HERE,
          base::Bind(&NotifyWrapper<ObserverStoreType, Method, Params>,
                     it->first, method, params));
    }
  }

 private:
  typedef base::internal::UnwrapTraits<ObserverStoreType> UnwrapTraits;

  ObserversListMap observers_;
};

class FileAccessObserver;
class FileChangeObserver;
class FileUpdateObserver;

typedef TaskRunnerBoundObserverList<FileAccessObserver> AccessObserverList;
typedef TaskRunnerBoundObserverList<FileChangeObserver> ChangeObserverList;
typedef TaskRunnerBoundObserverList<FileUpdateObserver> UpdateObserverList;

}  

#endif  
