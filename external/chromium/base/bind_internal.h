


// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_BIND_INTERNAL_H_
#define BASE_BIND_INTERNAL_H_
#pragma once

#include "base/bind_helpers.h"
#include "base/callback_internal.h"
#include "base/template_util.h"
#include "build/build_config.h"

#if defined(OS_WIN)
#include "base/bind_internal_win.h"
#endif

namespace base {
namespace internal {



template <typename Sig>
struct FunctionTraits;

template <typename R>
struct FunctionTraits<R(*)()> {
  typedef R (*NormalizedSig)();
  typedef false_type IsMethod;

};

template <typename R, typename T>
struct FunctionTraits<R(T::*)()> {
  typedef R (T::*NormalizedSig)();
  typedef true_type IsMethod;

  
  typedef T B1;

};

template <typename R, typename T>
struct FunctionTraits<R(T::*)() const> {
  typedef R (T::*NormalizedSig)();
  typedef true_type IsMethod;

  
  typedef T B1;

};

template <typename R, typename X1>
struct FunctionTraits<R(*)(X1)> {
  typedef R (*NormalizedSig)(X1);
  typedef false_type IsMethod;
  
  typedef X1 B1;

};

template <typename R, typename T, typename X1>
struct FunctionTraits<R(T::*)(X1)> {
  typedef R (T::*NormalizedSig)(X1);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;

};

template <typename R, typename T, typename X1>
struct FunctionTraits<R(T::*)(X1) const> {
  typedef R (T::*NormalizedSig)(X1);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;

};

template <typename R, typename X1, typename X2>
struct FunctionTraits<R(*)(X1, X2)> {
  typedef R (*NormalizedSig)(X1, X2);
  typedef false_type IsMethod;
  
  typedef X1 B1;
  typedef X2 B2;

};

template <typename R, typename T, typename X1, typename X2>
struct FunctionTraits<R(T::*)(X1, X2)> {
  typedef R (T::*NormalizedSig)(X1, X2);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;

};

template <typename R, typename T, typename X1, typename X2>
struct FunctionTraits<R(T::*)(X1, X2) const> {
  typedef R (T::*NormalizedSig)(X1, X2);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;

};

template <typename R, typename X1, typename X2, typename X3>
struct FunctionTraits<R(*)(X1, X2, X3)> {
  typedef R (*NormalizedSig)(X1, X2, X3);
  typedef false_type IsMethod;
  
  typedef X1 B1;
  typedef X2 B2;
  typedef X3 B3;

};

template <typename R, typename T, typename X1, typename X2, typename X3>
struct FunctionTraits<R(T::*)(X1, X2, X3)> {
  typedef R (T::*NormalizedSig)(X1, X2, X3);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;
  typedef X3 B4;

};

template <typename R, typename T, typename X1, typename X2, typename X3>
struct FunctionTraits<R(T::*)(X1, X2, X3) const> {
  typedef R (T::*NormalizedSig)(X1, X2, X3);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;
  typedef X3 B4;

};

template <typename R, typename X1, typename X2, typename X3, typename X4>
struct FunctionTraits<R(*)(X1, X2, X3, X4)> {
  typedef R (*NormalizedSig)(X1, X2, X3, X4);
  typedef false_type IsMethod;
  
  typedef X1 B1;
  typedef X2 B2;
  typedef X3 B3;
  typedef X4 B4;

};

template <typename R, typename T, typename X1, typename X2, typename X3,
    typename X4>
struct FunctionTraits<R(T::*)(X1, X2, X3, X4)> {
  typedef R (T::*NormalizedSig)(X1, X2, X3, X4);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;
  typedef X3 B4;
  typedef X4 B5;

};

template <typename R, typename T, typename X1, typename X2, typename X3,
    typename X4>
struct FunctionTraits<R(T::*)(X1, X2, X3, X4) const> {
  typedef R (T::*NormalizedSig)(X1, X2, X3, X4);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;
  typedef X3 B4;
  typedef X4 B5;

};

template <typename R, typename X1, typename X2, typename X3, typename X4,
    typename X5>
struct FunctionTraits<R(*)(X1, X2, X3, X4, X5)> {
  typedef R (*NormalizedSig)(X1, X2, X3, X4, X5);
  typedef false_type IsMethod;
  
  typedef X1 B1;
  typedef X2 B2;
  typedef X3 B3;
  typedef X4 B4;
  typedef X5 B5;

};

template <typename R, typename T, typename X1, typename X2, typename X3,
    typename X4, typename X5>
struct FunctionTraits<R(T::*)(X1, X2, X3, X4, X5)> {
  typedef R (T::*NormalizedSig)(X1, X2, X3, X4, X5);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;
  typedef X3 B4;
  typedef X4 B5;
  typedef X5 B6;

};

template <typename R, typename T, typename X1, typename X2, typename X3,
    typename X4, typename X5>
struct FunctionTraits<R(T::*)(X1, X2, X3, X4, X5) const> {
  typedef R (T::*NormalizedSig)(X1, X2, X3, X4, X5);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;
  typedef X3 B4;
  typedef X4 B5;
  typedef X5 B6;

};

template <typename R, typename X1, typename X2, typename X3, typename X4,
    typename X5, typename X6>
struct FunctionTraits<R(*)(X1, X2, X3, X4, X5, X6)> {
  typedef R (*NormalizedSig)(X1, X2, X3, X4, X5, X6);
  typedef false_type IsMethod;
  
  typedef X1 B1;
  typedef X2 B2;
  typedef X3 B3;
  typedef X4 B4;
  typedef X5 B5;
  typedef X6 B6;

};

template <typename R, typename T, typename X1, typename X2, typename X3,
    typename X4, typename X5, typename X6>
struct FunctionTraits<R(T::*)(X1, X2, X3, X4, X5, X6)> {
  typedef R (T::*NormalizedSig)(X1, X2, X3, X4, X5, X6);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;
  typedef X3 B4;
  typedef X4 B5;
  typedef X5 B6;
  typedef X6 B7;

};

template <typename R, typename T, typename X1, typename X2, typename X3,
    typename X4, typename X5, typename X6>
struct FunctionTraits<R(T::*)(X1, X2, X3, X4, X5, X6) const> {
  typedef R (T::*NormalizedSig)(X1, X2, X3, X4, X5, X6);
  typedef true_type IsMethod;

  
  typedef T B1;
  typedef X1 B2;
  typedef X2 B3;
  typedef X3 B4;
  typedef X4 B5;
  typedef X5 B6;
  typedef X6 B7;

};


template <typename StorageType, typename NormalizedSig>
struct Invoker0;

template <typename StorageType, typename R>
struct Invoker0<StorageType, R(*)()> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_();
  }
};

template <typename StorageType, typename R,typename X1>
struct Invoker0<StorageType, R(*)(X1)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X1>::ForwardType x1) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(x1);
  }
};

template <typename StorageType, typename R,typename X1, typename X2>
struct Invoker0<StorageType, R(*)(X1, X2)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X1>::ForwardType x1,
      typename internal::ParamTraits<X2>::ForwardType x2) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(x1, x2);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3>
struct Invoker0<StorageType, R(*)(X1, X2, X3)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X1>::ForwardType x1,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(x1, x2, x3);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4>
struct Invoker0<StorageType, R(*)(X1, X2, X3, X4)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X1>::ForwardType x1,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(x1, x2, x3, x4);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5>
struct Invoker0<StorageType, R(*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X1>::ForwardType x1,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(x1, x2, x3, x4, x5);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5, typename X6>
struct Invoker0<StorageType, R(*)(X1, X2, X3, X4, X5, X6)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X1>::ForwardType x1,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5,
      typename internal::ParamTraits<X6>::ForwardType x6) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(x1, x2, x3, x4, x5, x6);
  }
};

template <typename StorageType, typename NormalizedSig>
struct Invoker1;

template <typename StorageType, typename R,typename X1>
struct Invoker1<StorageType, R(*)(X1)> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_));
  }
};

template <typename StorageType, typename R, typename T>
struct Invoker1<StorageType, R(T::*)()> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)();
  }
};

template <typename StorageType, typename R,typename X1, typename X2>
struct Invoker1<StorageType, R(*)(X1, X2)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X2>::ForwardType x2) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), x2);
  }
};

template <typename StorageType, typename R, typename T, typename X1>
struct Invoker1<StorageType, R(T::*)(X1)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X1>::ForwardType x1) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(x1);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3>
struct Invoker1<StorageType, R(*)(X1, X2, X3)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), x2, x3);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2>
struct Invoker1<StorageType, R(T::*)(X1, X2)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X1>::ForwardType x1,
      typename internal::ParamTraits<X2>::ForwardType x2) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(x1, x2);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4>
struct Invoker1<StorageType, R(*)(X1, X2, X3, X4)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), x2, x3, x4);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3>
struct Invoker1<StorageType, R(T::*)(X1, X2, X3)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X1>::ForwardType x1,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(x1, x2, x3);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5>
struct Invoker1<StorageType, R(*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), x2, x3, x4, x5);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3, typename X4>
struct Invoker1<StorageType, R(T::*)(X1, X2, X3, X4)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X1>::ForwardType x1,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(x1, x2, x3, x4);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5, typename X6>
struct Invoker1<StorageType, R(*)(X1, X2, X3, X4, X5, X6)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5,
      typename internal::ParamTraits<X6>::ForwardType x6) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), x2, x3, x4, x5, x6);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3, typename X4, typename X5>
struct Invoker1<StorageType, R(T::*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X1>::ForwardType x1,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(x1, x2, x3, x4, x5);
  }
};

template <typename StorageType, typename NormalizedSig>
struct Invoker2;

template <typename StorageType, typename R,typename X1, typename X2>
struct Invoker2<StorageType, R(*)(X1, X2)> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_));
  }
};

template <typename StorageType, typename R, typename T, typename X1>
struct Invoker2<StorageType, R(T::*)(X1)> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_));
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3>
struct Invoker2<StorageType, R(*)(X1, X2, X3)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X3>::ForwardType x3) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_), x3);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2>
struct Invoker2<StorageType, R(T::*)(X1, X2)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X2>::ForwardType x2) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_), x2);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4>
struct Invoker2<StorageType, R(*)(X1, X2, X3, X4)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_), x3, x4);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3>
struct Invoker2<StorageType, R(T::*)(X1, X2, X3)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_), x2, x3);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5>
struct Invoker2<StorageType, R(*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_), x3, x4, x5);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3, typename X4>
struct Invoker2<StorageType, R(T::*)(X1, X2, X3, X4)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_), x2, x3,
        x4);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5, typename X6>
struct Invoker2<StorageType, R(*)(X1, X2, X3, X4, X5, X6)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5,
      typename internal::ParamTraits<X6>::ForwardType x6) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_), x3, x4, x5,
        x6);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3, typename X4, typename X5>
struct Invoker2<StorageType, R(T::*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X2>::ForwardType x2,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_), x2, x3,
        x4, x5);
  }
};

template <typename StorageType, typename NormalizedSig>
struct Invoker3;

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3>
struct Invoker3<StorageType, R(*)(X1, X2, X3)> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_),
        Unwrap(invoker->p3_));
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2>
struct Invoker3<StorageType, R(T::*)(X1, X2)> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_),
        Unwrap(invoker->p3_));
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4>
struct Invoker3<StorageType, R(*)(X1, X2, X3, X4)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X4>::ForwardType x4) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), x4);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3>
struct Invoker3<StorageType, R(T::*)(X1, X2, X3)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X3>::ForwardType x3) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), x3);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5>
struct Invoker3<StorageType, R(*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), x4, x5);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3, typename X4>
struct Invoker3<StorageType, R(T::*)(X1, X2, X3, X4)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), x3, x4);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5, typename X6>
struct Invoker3<StorageType, R(*)(X1, X2, X3, X4, X5, X6)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5,
      typename internal::ParamTraits<X6>::ForwardType x6) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), x4, x5, x6);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3, typename X4, typename X5>
struct Invoker3<StorageType, R(T::*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X3>::ForwardType x3,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), x3, x4, x5);
  }
};

template <typename StorageType, typename NormalizedSig>
struct Invoker4;

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4>
struct Invoker4<StorageType, R(*)(X1, X2, X3, X4)> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_));
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3>
struct Invoker4<StorageType, R(T::*)(X1, X2, X3)> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_));
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5>
struct Invoker4<StorageType, R(*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X5>::ForwardType x5) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_), x5);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3, typename X4>
struct Invoker4<StorageType, R(T::*)(X1, X2, X3, X4)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X4>::ForwardType x4) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_), x4);
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5, typename X6>
struct Invoker4<StorageType, R(*)(X1, X2, X3, X4, X5, X6)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X5>::ForwardType x5,
      typename internal::ParamTraits<X6>::ForwardType x6) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_), x5, x6);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3, typename X4, typename X5>
struct Invoker4<StorageType, R(T::*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X4>::ForwardType x4,
      typename internal::ParamTraits<X5>::ForwardType x5) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_), x4, x5);
  }
};

template <typename StorageType, typename NormalizedSig>
struct Invoker5;

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5>
struct Invoker5<StorageType, R(*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_), Unwrap(invoker->p5_));
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3, typename X4>
struct Invoker5<StorageType, R(T::*)(X1, X2, X3, X4)> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_), Unwrap(invoker->p5_));
  }
};

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5, typename X6>
struct Invoker5<StorageType, R(*)(X1, X2, X3, X4, X5, X6)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X6>::ForwardType x6) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_), Unwrap(invoker->p5_), x6);
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3, typename X4, typename X5>
struct Invoker5<StorageType, R(T::*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base,
      typename internal::ParamTraits<X5>::ForwardType x5) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_), Unwrap(invoker->p5_), x5);
  }
};

template <typename StorageType, typename NormalizedSig>
struct Invoker6;

template <typename StorageType, typename R,typename X1, typename X2,
    typename X3, typename X4, typename X5, typename X6>
struct Invoker6<StorageType, R(*)(X1, X2, X3, X4, X5, X6)> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return invoker->f_(Unwrap(invoker->p1_), Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_), Unwrap(invoker->p5_),
        Unwrap(invoker->p6_));
  }
};

template <typename StorageType, typename R, typename T, typename X1,
    typename X2, typename X3, typename X4, typename X5>
struct Invoker6<StorageType, R(T::*)(X1, X2, X3, X4, X5)> {
  static R DoInvoke(InvokerStorageBase* base) {
    StorageType* invoker = static_cast<StorageType*>(base);
    return (Unwrap(invoker->p1_)->*invoker->f_)(Unwrap(invoker->p2_),
        Unwrap(invoker->p3_), Unwrap(invoker->p4_), Unwrap(invoker->p5_),
        Unwrap(invoker->p6_));
  }
};



template <typename Sig>
class InvokerStorage0 : public InvokerStorageBase {
 public:
  typedef InvokerStorage0 StorageType;
  typedef FunctionTraits<Sig> TargetTraits;
  typedef Invoker0<StorageType, typename TargetTraits::NormalizedSig> Invoker;
  typedef typename TargetTraits::IsMethod IsMethod;



  InvokerStorage0(Sig f)
      : f_(f) {
  }

  virtual ~InvokerStorage0() {  }

  Sig f_;
};

template <typename Sig, typename P1>
class InvokerStorage1 : public InvokerStorageBase {
 public:
  typedef InvokerStorage1 StorageType;
  typedef FunctionTraits<Sig> TargetTraits;
  typedef Invoker1<StorageType, typename TargetTraits::NormalizedSig> Invoker;
  typedef typename TargetTraits::IsMethod IsMethod;

  
  
  
  COMPILE_ASSERT(IsMethod::value ||
                 !internal::UnsafeBindtoRefCountedArg<P1>::value,
                 p1_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!IsMethod::value || !is_array<P1>::value,
                 first_bound_argument_to_method_cannot_be_array);

  
  
  
  
  
  COMPILE_ASSERT(
      !( is_non_const_reference<typename TargetTraits::B1>::value ),
      do_not_bind_functions_with_nonconst_ref);


  InvokerStorage1(Sig f, const P1& p1)
      : f_(f), p1_(static_cast<typename ParamTraits<P1>::StorageType>(p1)) {
    MaybeRefcount<IsMethod, P1>::AddRef(p1_);
  }

  virtual ~InvokerStorage1() {
    MaybeRefcount<IsMethod, P1>::Release(p1_);
  }

  Sig f_;
  typename ParamTraits<P1>::StorageType p1_;
};

template <typename Sig, typename P1, typename P2>
class InvokerStorage2 : public InvokerStorageBase {
 public:
  typedef InvokerStorage2 StorageType;
  typedef FunctionTraits<Sig> TargetTraits;
  typedef Invoker2<StorageType, typename TargetTraits::NormalizedSig> Invoker;
  typedef typename TargetTraits::IsMethod IsMethod;

  
  
  
  COMPILE_ASSERT(IsMethod::value ||
                 !internal::UnsafeBindtoRefCountedArg<P1>::value,
                 p1_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!IsMethod::value || !is_array<P1>::value,
                 first_bound_argument_to_method_cannot_be_array);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P2>::value,
                 p2_is_refcounted_type_and_needs_scoped_refptr);

  
  
  
  
  
  COMPILE_ASSERT(
      !( is_non_const_reference<typename TargetTraits::B1>::value ||
          is_non_const_reference<typename TargetTraits::B2>::value ),
      do_not_bind_functions_with_nonconst_ref);


  InvokerStorage2(Sig f, const P1& p1, const P2& p2)
      : f_(f), p1_(static_cast<typename ParamTraits<P1>::StorageType>(p1)),
          p2_(static_cast<typename ParamTraits<P2>::StorageType>(p2)) {
    MaybeRefcount<IsMethod, P1>::AddRef(p1_);
  }

  virtual ~InvokerStorage2() {
    MaybeRefcount<IsMethod, P1>::Release(p1_);
  }

  Sig f_;
  typename ParamTraits<P1>::StorageType p1_;
  typename ParamTraits<P2>::StorageType p2_;
};

template <typename Sig, typename P1, typename P2, typename P3>
class InvokerStorage3 : public InvokerStorageBase {
 public:
  typedef InvokerStorage3 StorageType;
  typedef FunctionTraits<Sig> TargetTraits;
  typedef Invoker3<StorageType, typename TargetTraits::NormalizedSig> Invoker;
  typedef typename TargetTraits::IsMethod IsMethod;

  
  
  
  COMPILE_ASSERT(IsMethod::value ||
                 !internal::UnsafeBindtoRefCountedArg<P1>::value,
                 p1_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!IsMethod::value || !is_array<P1>::value,
                 first_bound_argument_to_method_cannot_be_array);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P2>::value,
                 p2_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P3>::value,
                 p3_is_refcounted_type_and_needs_scoped_refptr);

  
  
  
  
  
  COMPILE_ASSERT(
      !( is_non_const_reference<typename TargetTraits::B1>::value ||
          is_non_const_reference<typename TargetTraits::B2>::value ||
          is_non_const_reference<typename TargetTraits::B3>::value ),
      do_not_bind_functions_with_nonconst_ref);


  InvokerStorage3(Sig f, const P1& p1, const P2& p2, const P3& p3)
      : f_(f), p1_(static_cast<typename ParamTraits<P1>::StorageType>(p1)),
          p2_(static_cast<typename ParamTraits<P2>::StorageType>(p2)),
          p3_(static_cast<typename ParamTraits<P3>::StorageType>(p3)) {
    MaybeRefcount<IsMethod, P1>::AddRef(p1_);
  }

  virtual ~InvokerStorage3() {
    MaybeRefcount<IsMethod, P1>::Release(p1_);
  }

  Sig f_;
  typename ParamTraits<P1>::StorageType p1_;
  typename ParamTraits<P2>::StorageType p2_;
  typename ParamTraits<P3>::StorageType p3_;
};

template <typename Sig, typename P1, typename P2, typename P3, typename P4>
class InvokerStorage4 : public InvokerStorageBase {
 public:
  typedef InvokerStorage4 StorageType;
  typedef FunctionTraits<Sig> TargetTraits;
  typedef Invoker4<StorageType, typename TargetTraits::NormalizedSig> Invoker;
  typedef typename TargetTraits::IsMethod IsMethod;

  
  
  
  COMPILE_ASSERT(IsMethod::value ||
                 !internal::UnsafeBindtoRefCountedArg<P1>::value,
                 p1_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!IsMethod::value || !is_array<P1>::value,
                 first_bound_argument_to_method_cannot_be_array);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P2>::value,
                 p2_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P3>::value,
                 p3_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P4>::value,
                 p4_is_refcounted_type_and_needs_scoped_refptr);

  
  
  
  
  
  COMPILE_ASSERT(
      !( is_non_const_reference<typename TargetTraits::B1>::value ||
          is_non_const_reference<typename TargetTraits::B2>::value ||
          is_non_const_reference<typename TargetTraits::B3>::value ||
          is_non_const_reference<typename TargetTraits::B4>::value ),
      do_not_bind_functions_with_nonconst_ref);


  InvokerStorage4(Sig f, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
      : f_(f), p1_(static_cast<typename ParamTraits<P1>::StorageType>(p1)),
          p2_(static_cast<typename ParamTraits<P2>::StorageType>(p2)),
          p3_(static_cast<typename ParamTraits<P3>::StorageType>(p3)),
          p4_(static_cast<typename ParamTraits<P4>::StorageType>(p4)) {
    MaybeRefcount<IsMethod, P1>::AddRef(p1_);
  }

  virtual ~InvokerStorage4() {
    MaybeRefcount<IsMethod, P1>::Release(p1_);
  }

  Sig f_;
  typename ParamTraits<P1>::StorageType p1_;
  typename ParamTraits<P2>::StorageType p2_;
  typename ParamTraits<P3>::StorageType p3_;
  typename ParamTraits<P4>::StorageType p4_;
};

template <typename Sig, typename P1, typename P2, typename P3, typename P4,
    typename P5>
class InvokerStorage5 : public InvokerStorageBase {
 public:
  typedef InvokerStorage5 StorageType;
  typedef FunctionTraits<Sig> TargetTraits;
  typedef Invoker5<StorageType, typename TargetTraits::NormalizedSig> Invoker;
  typedef typename TargetTraits::IsMethod IsMethod;

  
  
  
  COMPILE_ASSERT(IsMethod::value ||
                 !internal::UnsafeBindtoRefCountedArg<P1>::value,
                 p1_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!IsMethod::value || !is_array<P1>::value,
                 first_bound_argument_to_method_cannot_be_array);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P2>::value,
                 p2_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P3>::value,
                 p3_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P4>::value,
                 p4_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P5>::value,
                 p5_is_refcounted_type_and_needs_scoped_refptr);

  
  
  
  
  
  COMPILE_ASSERT(
      !( is_non_const_reference<typename TargetTraits::B1>::value ||
          is_non_const_reference<typename TargetTraits::B2>::value ||
          is_non_const_reference<typename TargetTraits::B3>::value ||
          is_non_const_reference<typename TargetTraits::B4>::value ||
          is_non_const_reference<typename TargetTraits::B5>::value ),
      do_not_bind_functions_with_nonconst_ref);


  InvokerStorage5(Sig f, const P1& p1, const P2& p2, const P3& p3,
      const P4& p4, const P5& p5)
      : f_(f), p1_(static_cast<typename ParamTraits<P1>::StorageType>(p1)),
          p2_(static_cast<typename ParamTraits<P2>::StorageType>(p2)),
          p3_(static_cast<typename ParamTraits<P3>::StorageType>(p3)),
          p4_(static_cast<typename ParamTraits<P4>::StorageType>(p4)),
          p5_(static_cast<typename ParamTraits<P5>::StorageType>(p5)) {
    MaybeRefcount<IsMethod, P1>::AddRef(p1_);
  }

  virtual ~InvokerStorage5() {
    MaybeRefcount<IsMethod, P1>::Release(p1_);
  }

  Sig f_;
  typename ParamTraits<P1>::StorageType p1_;
  typename ParamTraits<P2>::StorageType p2_;
  typename ParamTraits<P3>::StorageType p3_;
  typename ParamTraits<P4>::StorageType p4_;
  typename ParamTraits<P5>::StorageType p5_;
};

template <typename Sig, typename P1, typename P2, typename P3, typename P4,
    typename P5, typename P6>
class InvokerStorage6 : public InvokerStorageBase {
 public:
  typedef InvokerStorage6 StorageType;
  typedef FunctionTraits<Sig> TargetTraits;
  typedef Invoker6<StorageType, typename TargetTraits::NormalizedSig> Invoker;
  typedef typename TargetTraits::IsMethod IsMethod;

  
  
  
  COMPILE_ASSERT(IsMethod::value ||
                 !internal::UnsafeBindtoRefCountedArg<P1>::value,
                 p1_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!IsMethod::value || !is_array<P1>::value,
                 first_bound_argument_to_method_cannot_be_array);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P2>::value,
                 p2_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P3>::value,
                 p3_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P4>::value,
                 p4_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P5>::value,
                 p5_is_refcounted_type_and_needs_scoped_refptr);
  COMPILE_ASSERT(!internal::UnsafeBindtoRefCountedArg<P6>::value,
                 p6_is_refcounted_type_and_needs_scoped_refptr);

  
  
  
  
  
  COMPILE_ASSERT(
      !( is_non_const_reference<typename TargetTraits::B1>::value ||
          is_non_const_reference<typename TargetTraits::B2>::value ||
          is_non_const_reference<typename TargetTraits::B3>::value ||
          is_non_const_reference<typename TargetTraits::B4>::value ||
          is_non_const_reference<typename TargetTraits::B5>::value ||
          is_non_const_reference<typename TargetTraits::B6>::value ),
      do_not_bind_functions_with_nonconst_ref);


  InvokerStorage6(Sig f, const P1& p1, const P2& p2, const P3& p3,
      const P4& p4, const P5& p5, const P6& p6)
      : f_(f), p1_(static_cast<typename ParamTraits<P1>::StorageType>(p1)),
          p2_(static_cast<typename ParamTraits<P2>::StorageType>(p2)),
          p3_(static_cast<typename ParamTraits<P3>::StorageType>(p3)),
          p4_(static_cast<typename ParamTraits<P4>::StorageType>(p4)),
          p5_(static_cast<typename ParamTraits<P5>::StorageType>(p5)),
          p6_(static_cast<typename ParamTraits<P6>::StorageType>(p6)) {
    MaybeRefcount<IsMethod, P1>::AddRef(p1_);
  }

  virtual ~InvokerStorage6() {
    MaybeRefcount<IsMethod, P1>::Release(p1_);
  }

  Sig f_;
  typename ParamTraits<P1>::StorageType p1_;
  typename ParamTraits<P2>::StorageType p2_;
  typename ParamTraits<P3>::StorageType p3_;
  typename ParamTraits<P4>::StorageType p4_;
  typename ParamTraits<P5>::StorageType p5_;
  typename ParamTraits<P6>::StorageType p6_;
};

}  
}  

#endif  
