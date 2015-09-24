// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_OUTPUT_TRAITS_H_
#define PPAPI_CPP_OUTPUT_TRAITS_H_

#include <vector>

#include "ppapi/c/pp_resource.h"
#include "ppapi/cpp/array_output.h"
#include "ppapi/cpp/resource.h"


struct PP_Var;

namespace pp {

class Var;

namespace internal {

template<typename, typename> struct IsSame {
  static bool const value = false;
};
template<typename A> struct IsSame<A, A> {
  static bool const value = true;
};
template<typename Base, typename Derived> struct IsBaseOf {
 private:
  
  
  
  typedef char (*EnsureTypesAreComplete)[sizeof(Base) + sizeof(Derived)];

  static Derived* CreateDerived();
  static char (&Check(Base*))[1];
  static char (&Check(...))[2];

 public:
  static bool const value = sizeof Check(CreateDerived()) == 1 &&
                            !IsSame<Base const, void const>::value;
};

template <class T, bool P> struct InheritIf {};
template <class T> struct InheritIf<T, true> : public T {};


template<typename T>
struct GenericCallbackOutputTraits {
  
  
  typedef T* APIArgType;

  
  
  
  
  typedef T StorageType;

  
  
  
  static inline APIArgType StorageToAPIArg(StorageType& t) { return &t; }

  
  
  
  static inline T& StorageToPluginArg(StorageType& t) { return t; }

  
  
  static inline void Initialize(StorageType* ) {}
};

template<typename T>
struct ResourceCallbackOutputTraits {
  
  typedef PP_Resource* APIArgType;
  typedef PP_Resource StorageType;

  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return &t;
  }

  
  
  
  
  
  static inline T StorageToPluginArg(StorageType& t) {
    return T(PASS_REF, t);
  }

  static inline void Initialize(StorageType* t) {
    *t = 0;
  }
};

template<typename T>
struct CallbackOutputTraits
    : public InheritIf<GenericCallbackOutputTraits<T>,
                       !IsBaseOf<Resource, T>::value>,
      public InheritIf<ResourceCallbackOutputTraits<T>,
                       IsBaseOf<Resource, T>::value> {
};

template<>
struct CallbackOutputTraits<Var> {
  
  typedef PP_Var* APIArgType;
  typedef PP_Var StorageType;

  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return &t;
  }

  
  
  
  static inline pp::Var StorageToPluginArg(StorageType& t) {
    return Var(PASS_REF, t);
  }

  static inline void Initialize(StorageType* t) {
    *t = PP_MakeUndefined();
  }
};

template<>
struct CallbackOutputTraits<bool> {
  
  typedef PP_Bool* APIArgType;
  typedef PP_Bool StorageType;

  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return &t;
  }

  
  static inline bool StorageToPluginArg(StorageType& t) {
    return PP_ToBool(t);
  }

  static inline void Initialize(StorageType* t) {
    *t = PP_FALSE;
  }
};


template<typename T>
struct GenericVectorCallbackOutputTraits {
  
  typedef PP_ArrayOutput APIArgType;

  
  
  typedef ArrayOutputAdapterWithStorage<T> StorageType;

  
  
  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return t.pp_array_output();
  }

  
  static inline std::vector<T>& StorageToPluginArg(StorageType& t) {
    return t.output();
  }

  static inline void Initialize(StorageType* ) {}
};

template<typename T>
struct ResourceVectorCallbackOutputTraits {
  typedef PP_ArrayOutput APIArgType;
  typedef ResourceArrayOutputAdapterWithStorage<T> StorageType;

  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return t.pp_array_output();
  }
  static inline std::vector<T>& StorageToPluginArg(StorageType& t) {
    return t.output();
  }

  static inline void Initialize(StorageType* ) {}
};

template<typename T>
struct CallbackOutputTraits< std::vector<T> >
    : public InheritIf<GenericVectorCallbackOutputTraits<T>,
                       !IsBaseOf<Resource, T>::value>,
      public InheritIf<ResourceVectorCallbackOutputTraits<T>,
                       IsBaseOf<Resource, T>::value> {
};

template<>
struct CallbackOutputTraits< std::vector<pp::Var> > {
  
  typedef PP_ArrayOutput APIArgType;

  
  
  typedef VarArrayOutputAdapterWithStorage StorageType;

  
  
  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return t.pp_array_output();
  }

  
  static inline std::vector<pp::Var>& StorageToPluginArg(StorageType& t) {
    return t.output();
  }

  static inline void Initialize(StorageType* ) {}
};

}  
}  

#endif  
