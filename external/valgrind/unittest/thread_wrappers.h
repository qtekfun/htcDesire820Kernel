/*
  This file is part of ThreadSanitizer, a dynamic data race detector.

  Copyright (C) 2008-2009 Google Inc
     opensource@google.com

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307, USA.

  The GNU General Public License is contained in the file COPYING.
*/


#ifndef THREAD_WRAPPERS_H
#define THREAD_WRAPPERS_H

#include <assert.h>
#include <limits.h>   
#include <queue>
#include <stdio.h>
#include <string>
#include <time.h>

#include "dynamic_annotations.h"

using namespace std;

#ifdef NDEBUG
# error "Pleeease, do not define NDEBUG"
#endif

#ifdef WIN32
# define CHECK(x) do { if (!(x)) { \
   fprintf(stderr, "Assertion failed: %s (%s:%d) %s\n", \
          __FUNCTION__, __FILE__, __LINE__, #x); \
   exit(1); }} while (0)
#else
# define CHECK assert
#endif

class Condition {
 public:
  typedef bool (*func_t)(void*);

  template <typename T>
  Condition(bool (*func)(T*), T* arg)
  : func_(reinterpret_cast<func_t>(func)), arg_(arg) {}

  Condition(bool (*func)())
  : func_(reinterpret_cast<func_t>(func)), arg_(NULL) {}

  bool Eval() { return func_(arg_); }
 private:
  func_t func_;
  void *arg_;
};

static int AtomicIncrement(volatile int *value, int increment);
static int GetTimeInMs();

class CondVar;
class MyThread;
class Mutex;

#ifndef WIN32
#include "thread_wrappers_pthread.h"
#else
#include "thread_wrappers_win.h"
#endif

class ProducerConsumerQueue {
 public:
  ProducerConsumerQueue(int unused) {
    
  }
  ~ProducerConsumerQueue() {
    CHECK(q_.empty());
    
  }

  
  void Put(void *item) {
    mu_.Lock();
      q_.push(item);
      ANNOTATE_CONDVAR_SIGNAL(&mu_); 
      
    mu_.Unlock();
  }

  
  
  void *Get() {
    mu_.LockWhen(Condition(IsQueueNotEmpty, &q_));
      void * item;
      bool ok = TryGetInternal(&item);
      CHECK(ok);
    mu_.Unlock();
    return item;
  }

  
  
  
  bool TryGet(void **res) {
    mu_.Lock();
      bool ok = TryGetInternal(res);
    mu_.Unlock();
    return ok;
  }

 private:
  Mutex mu_;
  std::queue<void*> q_; 

  
  bool TryGetInternal(void ** item_ptr) {
    if (q_.empty())
      return false;
    *item_ptr = q_.front();
    q_.pop();
    
    return true;
  }

  static bool IsQueueNotEmpty(std::queue<void*> * queue) {
     return !queue->empty();
  }
};

struct Closure {
  typedef void (*F0)();
  typedef void (*F1)(void *arg1);
  typedef void (*F2)(void *arg1, void *arg2);
  int  n_params;
  void *f;
  void *param1;
  void *param2;

  void Execute() {
    if (n_params == 0) {
      (F0(f))();
    } else if (n_params == 1) {
      (F1(f))(param1);
    } else {
      CHECK(n_params == 2);
      (F2(f))(param1, param2);
    }
    delete this;
  }
};

static Closure *NewCallback(void (*f)()) {
  Closure *res = new Closure;
  res->n_params = 0;
  res->f = (void*)(f);
  res->param1 = NULL;
  res->param2 = NULL;
  return res;
}

template <class P1>
Closure *NewCallback(void (*f)(P1), P1 p1) {
  CHECK(sizeof(P1) <= sizeof(void*));
  Closure *res = new Closure;
  res->n_params = 1;
  res->f = (void*)(f);
  res->param1 = (void*)(intptr_t)p1;
  res->param2 = NULL;
  return res;
}

template <class P1, class P2>
Closure *NewCallback(void (*f)(P1, P2), P1 p1, P2 p2) {
  CHECK(sizeof(P1) <= sizeof(void*));
  CHECK(sizeof(P2) <= sizeof(void*));
  Closure *res = new Closure;
  res->n_params = 2;
  res->f = (void*)(f);
  res->param1 = (void*)p1;
  res->param2 = (void*)p2;
  return res;
}

class ThreadPool {
 public:
  
  explicit ThreadPool(int n_threads)
    : queue_(INT_MAX) {
    for (int i = 0; i < n_threads; i++) {
      MyThread *thread = new MyThread(&ThreadPool::Worker, this);
      workers_.push_back(thread);
    }
  }

  
  void StartWorkers() {
    for (size_t i = 0; i < workers_.size(); i++) {
      workers_[i]->Start();
    }
  }

  
  void Add(Closure *closure) {
    queue_.Put(closure);
  }

  int num_threads() { return workers_.size();}

  
  ~ThreadPool() {
    for (size_t i = 0; i < workers_.size(); i++) {
      Add(NULL);
    }
    for (size_t i = 0; i < workers_.size(); i++) {
      workers_[i]->Join();
      delete workers_[i];
    }
  }
 private:
  std::vector<MyThread*>   workers_;
  ProducerConsumerQueue  queue_;

  static void *Worker(void *p) {
    ThreadPool *pool = reinterpret_cast<ThreadPool*>(p);
    while (true) {
      Closure *closure = reinterpret_cast<Closure*>(pool->queue_.Get());
      if(closure == NULL) {
        return NULL;
      }
      closure->Execute();
    }
  }
};

class MutexLock {  
 public:
  MutexLock(Mutex *mu)
    : mu_(mu) {
    mu_->Lock();
  }
  ~MutexLock() {
    mu_->Unlock();
  }
 private:
  Mutex *mu_;
};

class BlockingCounter {
 public:
  explicit BlockingCounter(int initial_count) :
    count_(initial_count) {}
  bool DecrementCount() {
    MutexLock lock(&mu_);
    count_--;
    return count_ == 0;
  }
  void Wait() {
    mu_.LockWhen(Condition(&IsZero, &count_));
    mu_.Unlock();
  }
 private:
  static bool IsZero(int *arg) { return *arg == 0; }
  Mutex mu_;
  int count_;
};


#endif 