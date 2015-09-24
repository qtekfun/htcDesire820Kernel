// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TASK_H_
#define BASE_TASK_H_
#pragma once

#include "base/base_api.h"
#include "base/memory/raw_scoped_refptr_mismatch_checker.h"
#include "base/memory/weak_ptr.h"
#include "base/tracked.h"
#include "base/tuple.h"


class BASE_API Task : public tracked_objects::Tracked {
 public:
  Task();
  virtual ~Task();

  
  virtual void Run() = 0;
};

class BASE_API CancelableTask : public Task {
 public:
  CancelableTask();
  virtual ~CancelableTask();

  
  virtual void Cancel() = 0;
};


template<class T>
class ScopedRunnableMethodFactory {
 public:
  explicit ScopedRunnableMethodFactory(T* object) : weak_factory_(object) {
  }

  template <class Method>
  inline CancelableTask* NewRunnableMethod(Method method) {
    return new RunnableMethod<Method, Tuple0>(
        weak_factory_.GetWeakPtr(), method, MakeTuple());
  }

  template <class Method, class A>
  inline CancelableTask* NewRunnableMethod(Method method, const A& a) {
    return new RunnableMethod<Method, Tuple1<A> >(
        weak_factory_.GetWeakPtr(), method, MakeTuple(a));
  }

  template <class Method, class A, class B>
  inline CancelableTask* NewRunnableMethod(Method method, const A& a,
                                           const B& b) {
    return new RunnableMethod<Method, Tuple2<A, B> >(
        weak_factory_.GetWeakPtr(), method, MakeTuple(a, b));
  }

  template <class Method, class A, class B, class C>
  inline CancelableTask* NewRunnableMethod(Method method,
                                           const A& a,
                                           const B& b,
                                           const C& c) {
    return new RunnableMethod<Method, Tuple3<A, B, C> >(
        weak_factory_.GetWeakPtr(), method, MakeTuple(a, b, c));
  }

  template <class Method, class A, class B, class C, class D>
  inline CancelableTask* NewRunnableMethod(Method method,
                                           const A& a,
                                           const B& b,
                                           const C& c,
                                           const D& d) {
    return new RunnableMethod<Method, Tuple4<A, B, C, D> >(
        weak_factory_.GetWeakPtr(), method, MakeTuple(a, b, c, d));
  }

  template <class Method, class A, class B, class C, class D, class E>
  inline CancelableTask* NewRunnableMethod(Method method,
                                           const A& a,
                                           const B& b,
                                           const C& c,
                                           const D& d,
                                           const E& e) {
    return new RunnableMethod<Method, Tuple5<A, B, C, D, E> >(
        weak_factory_.GetWeakPtr(), method, MakeTuple(a, b, c, d, e));
  }

  void RevokeAll() { weak_factory_.InvalidateWeakPtrs(); }

  bool empty() const { return !weak_factory_.HasWeakPtrs(); }

 protected:
  template <class Method, class Params>
  class RunnableMethod : public CancelableTask {
   public:
    RunnableMethod(const base::WeakPtr<T>& obj,
                   Method meth,
                   const Params& params)
        : obj_(obj),
          meth_(meth),
          params_(params) {
      COMPILE_ASSERT(
          (base::internal::ParamsUseScopedRefptrCorrectly<Params>::value),
          badscopedrunnablemethodparams);
    }

    virtual void Run() {
      if (obj_)
        DispatchToMethod(obj_.get(), meth_, params_);
    }

    virtual void Cancel() {
      obj_.reset();
    }

   private:
    base::WeakPtr<T> obj_;
    Method meth_;
    Params params_;

    DISALLOW_COPY_AND_ASSIGN(RunnableMethod);
  };

 private:
  base::WeakPtrFactory<T> weak_factory_;
};


template<class T>
class DeleteTask : public CancelableTask {
 public:
  explicit DeleteTask(const T* obj) : obj_(obj) {
  }
  virtual void Run() {
    delete obj_;
  }
  virtual void Cancel() {
    obj_ = NULL;
  }

 private:
  const T* obj_;
};

template<class T>
class ReleaseTask : public CancelableTask {
 public:
  explicit ReleaseTask(const T* obj) : obj_(obj) {
  }
  virtual void Run() {
    if (obj_)
      obj_->Release();
  }
  virtual void Cancel() {
    obj_ = NULL;
  }

 private:
  const T* obj_;
};


template <class T>
struct RunnableMethodTraits {
  RunnableMethodTraits() {
#ifndef NDEBUG
    origin_thread_id_ = base::PlatformThread::CurrentId();
#endif
  }

  ~RunnableMethodTraits() {
#ifndef NDEBUG
    
    
    if (origin_thread_id_ != base::PlatformThread::CurrentId())
      DCHECK(T::ImplementsThreadSafeReferenceCounting());
#endif
  }

  void RetainCallee(T* obj) {
#ifndef NDEBUG
    
    
    
    obj->AddRef();
    obj->Release();
#endif
    obj->AddRef();
  }

  void ReleaseCallee(T* obj) {
    obj->Release();
  }

 private:
#ifndef NDEBUG
  base::PlatformThreadId origin_thread_id_;
#endif
};

#define DISABLE_RUNNABLE_METHOD_REFCOUNT(TypeName) \
  template <>                                      \
  struct RunnableMethodTraits<TypeName> {          \
    void RetainCallee(TypeName* manager) {}        \
    void ReleaseCallee(TypeName* manager) {}       \
  }



template <class T, class Method, class Params>
class RunnableMethod : public CancelableTask {
 public:
  RunnableMethod(T* obj, Method meth, const Params& params)
      : obj_(obj), meth_(meth), params_(params) {
    traits_.RetainCallee(obj_);
    COMPILE_ASSERT(
        (base::internal::ParamsUseScopedRefptrCorrectly<Params>::value),
        badrunnablemethodparams);
  }

  ~RunnableMethod() {
    ReleaseCallee();
  }

  virtual void Run() {
    if (obj_)
      DispatchToMethod(obj_, meth_, params_);
  }

  virtual void Cancel() {
    ReleaseCallee();
  }

 private:
  void ReleaseCallee() {
    T* obj = obj_;
    obj_ = NULL;
    if (obj)
      traits_.ReleaseCallee(obj);
  }

  T* obj_;
  Method meth_;
  Params params_;
  RunnableMethodTraits<T> traits_;
};

template <class T, class Method>
inline CancelableTask* NewRunnableMethod(T* object, Method method) {
  return new RunnableMethod<T, Method, Tuple0>(object, method, MakeTuple());
}

template <class T, class Method, class A>
inline CancelableTask* NewRunnableMethod(T* object, Method method, const A& a) {
  return new RunnableMethod<T, Method, Tuple1<A> >(object,
                                                   method,
                                                   MakeTuple(a));
}

template <class T, class Method, class A, class B>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
const A& a, const B& b) {
  return new RunnableMethod<T, Method, Tuple2<A, B> >(object, method,
                                                      MakeTuple(a, b));
}

template <class T, class Method, class A, class B, class C>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
                                          const A& a, const B& b, const C& c) {
  return new RunnableMethod<T, Method, Tuple3<A, B, C> >(object, method,
                                                         MakeTuple(a, b, c));
}

template <class T, class Method, class A, class B, class C, class D>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
                                          const A& a, const B& b,
                                          const C& c, const D& d) {
  return new RunnableMethod<T, Method, Tuple4<A, B, C, D> >(object, method,
                                                            MakeTuple(a, b,
                                                                      c, d));
}

template <class T, class Method, class A, class B, class C, class D, class E>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
                                          const A& a, const B& b,
                                          const C& c, const D& d, const E& e) {
  return new RunnableMethod<T,
                            Method,
                            Tuple5<A, B, C, D, E> >(object,
                                                    method,
                                                    MakeTuple(a, b, c, d, e));
}

template <class T, class Method, class A, class B, class C, class D, class E,
          class F>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
                                          const A& a, const B& b,
                                          const C& c, const D& d, const E& e,
                                          const F& f) {
  return new RunnableMethod<T,
                            Method,
                            Tuple6<A, B, C, D, E, F> >(object,
                                                       method,
                                                       MakeTuple(a, b, c, d, e,
                                                                 f));
}

template <class T, class Method, class A, class B, class C, class D, class E,
          class F, class G>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
                                         const A& a, const B& b,
                                         const C& c, const D& d, const E& e,
                                         const F& f, const G& g) {
  return new RunnableMethod<T,
                            Method,
                            Tuple7<A, B, C, D, E, F, G> >(object,
                                                          method,
                                                          MakeTuple(a, b, c, d,
                                                                    e, f, g));
}

template <class T, class Method, class A, class B, class C, class D, class E,
          class F, class G, class H>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
                                         const A& a, const B& b,
                                         const C& c, const D& d, const E& e,
                                         const F& f, const G& g, const H& h) {
  return new RunnableMethod<T,
                            Method,
                            Tuple8<A, B, C, D, E, F, G, H> >(object,
                                                             method,
                                                             MakeTuple(a, b, c,
                                                                       d, e, f,
                                                                       g, h));
}


template <class Function, class Params>
class RunnableFunction : public CancelableTask {
 public:
  RunnableFunction(Function function, const Params& params)
      : function_(function), params_(params) {
    COMPILE_ASSERT(
        (base::internal::ParamsUseScopedRefptrCorrectly<Params>::value),
        badrunnablefunctionparams);
  }

  ~RunnableFunction() {
  }

  virtual void Run() {
    if (function_)
      DispatchToFunction(function_, params_);
  }

  virtual void Cancel() {
  }

 private:
  Function function_;
  Params params_;
};

template <class Function>
inline CancelableTask* NewRunnableFunction(Function function) {
  return new RunnableFunction<Function, Tuple0>(function, MakeTuple());
}

template <class Function, class A>
inline CancelableTask* NewRunnableFunction(Function function, const A& a) {
  return new RunnableFunction<Function, Tuple1<A> >(function, MakeTuple(a));
}

template <class Function, class A, class B>
inline CancelableTask* NewRunnableFunction(Function function,
                                           const A& a, const B& b) {
  return new RunnableFunction<Function, Tuple2<A, B> >(function,
                                                       MakeTuple(a, b));
}

template <class Function, class A, class B, class C>
inline CancelableTask* NewRunnableFunction(Function function,
                                           const A& a, const B& b,
                                           const C& c) {
  return new RunnableFunction<Function, Tuple3<A, B, C> >(function,
                                                          MakeTuple(a, b, c));
}

template <class Function, class A, class B, class C, class D>
inline CancelableTask* NewRunnableFunction(Function function,
                                           const A& a, const B& b,
                                           const C& c, const D& d) {
  return new RunnableFunction<Function, Tuple4<A, B, C, D> >(function,
                                                             MakeTuple(a, b,
                                                                       c, d));
}

template <class Function, class A, class B, class C, class D, class E>
inline CancelableTask* NewRunnableFunction(Function function,
                                           const A& a, const B& b,
                                           const C& c, const D& d,
                                           const E& e) {
  return new RunnableFunction<Function, Tuple5<A, B, C, D, E> >(function,
                                                                MakeTuple(a, b,
                                                                          c, d,
                                                                          e));
}

template <class Function, class A, class B, class C, class D, class E,
          class F>
inline CancelableTask* NewRunnableFunction(Function function,
                                           const A& a, const B& b,
                                           const C& c, const D& d,
                                           const E& e, const F& f) {
  return new RunnableFunction<Function, Tuple6<A, B, C, D, E, F> >(function,
      MakeTuple(a, b, c, d, e, f));
}

template <class Function, class A, class B, class C, class D, class E,
          class F, class G>
inline CancelableTask* NewRunnableFunction(Function function,
                                           const A& a, const B& b,
                                           const C& c, const D& d,
                                           const E& e, const F& f,
                                           const G& g) {
  return new RunnableFunction<Function, Tuple7<A, B, C, D, E, F, G> >(function,
      MakeTuple(a, b, c, d, e, f, g));
}

template <class Function, class A, class B, class C, class D, class E,
          class F, class G, class H>
inline CancelableTask* NewRunnableFunction(Function function,
                                           const A& a, const B& b,
                                           const C& c, const D& d,
                                           const E& e, const F& f,
                                           const G& g, const H& h) {
  return new RunnableFunction<Function, Tuple8<A, B, C, D, E, F, G, H> >(
      function, MakeTuple(a, b, c, d, e, f, g, h));
}

#endif  
