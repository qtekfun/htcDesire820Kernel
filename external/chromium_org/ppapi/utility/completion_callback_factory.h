// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_UTILITY_COMPLETION_CALLBACK_FACTORY_H_
#define PPAPI_UTILITY_COMPLETION_CALLBACK_FACTORY_H_

#include "ppapi/cpp/completion_callback.h"
#include "ppapi/utility/completion_callback_factory_thread_traits.h"

namespace pp {


namespace internal {

template <typename T> struct TypeUnwrapper {
  typedef T StorageType;
};
template <typename T> struct TypeUnwrapper<T&> {
  typedef T StorageType;
};
template <typename T> struct TypeUnwrapper<const T&> {
  typedef T StorageType;
};

}  


/// in the plugin code, and the output parameter will still be written to!
template <typename T, typename ThreadTraits = ThreadSafeThreadTraits>
class CompletionCallbackFactory {
 public:

  
  
  
  
  
  
  
  
  
  explicit CompletionCallbackFactory(T* object = NULL)
      : object_(object) {
    
    
    InitBackPointer();
  }

  
  ~CompletionCallbackFactory() {
    
    
    ResetBackPointer();
  }

  
  
  void CancelAll() {
    typename ThreadTraits::AutoLock lock(lock_);

    ResetBackPointer();
    InitBackPointer();
  }

  
  
  
  
  
  
  
  
  
  
  
  void Initialize(T* object) {
    PP_DCHECK(object);
    PP_DCHECK(!object_);  
    object_ = object;
  }

  
  
  
  
  T* GetObject() {
    return object_;
  }

  
  
  
  
  
  
  
  
  template <typename Method>
  CompletionCallback NewCallback(Method method) {
    return NewCallbackHelper(new Dispatcher0<Method>(method));
  }

  
  
  
  
  
  
  
  
  
  
  
  template <typename Method>
  CompletionCallback NewOptionalCallback(Method method) {
    CompletionCallback cc = NewCallback(method);
    cc.set_flags(cc.flags() | PP_COMPLETIONCALLBACK_FLAG_OPTIONAL);
    return cc;
  }

  
  
  
  
  
  
  
  
  
  
  template <typename Output>
  CompletionCallbackWithOutput<
      typename internal::TypeUnwrapper<Output>::StorageType>
  NewCallbackWithOutput(void (T::*method)(int32_t, Output)) {
    typedef typename internal::TypeUnwrapper<Output>::StorageType
        OutputStorageType;
    typedef CompletionCallbackWithOutput<OutputStorageType> CallbackType;
    typedef DispatcherWithOutput0<
        typename CallbackType::TraitsType,
        OutputStorageType,
        void (T::*)(int32_t, Output)> DispatcherType;
    return NewCallbackWithOutputHelper<CallbackType>(
        new DispatcherType(method));
  }

  
  
  template <typename Output>
  ext::ExtCompletionCallbackWithOutput<
      typename internal::TypeUnwrapper<Output>::StorageType>
  NewExtCallbackWithOutput(void (T::*method)(int32_t, Output)) {
    typedef typename internal::TypeUnwrapper<Output>::StorageType
        OutputStorageType;
    typedef ext::ExtCompletionCallbackWithOutput<OutputStorageType>
        CallbackType;
    typedef DispatcherWithOutput0<
        typename CallbackType::TraitsType,
        OutputStorageType,
        void (T::*)(int32_t, Output)> DispatcherType;
    return NewCallbackWithOutputHelper<CallbackType>(
        new DispatcherType(method));
  }

  
  
  
  
  
  
  
  
  
  
  
  
  template <typename Method, typename A>
  CompletionCallback NewCallback(Method method, const A& a) {
    return NewCallbackHelper(new Dispatcher1<Method, A>(method, a));
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template <typename Method, typename A>
  CompletionCallback NewOptionalCallback(Method method, const A& a) {
    CompletionCallback cc = NewCallback(method, a);
    cc.set_flags(cc.flags() | PP_COMPLETIONCALLBACK_FLAG_OPTIONAL);
    return cc;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  template <typename Output, typename A>
  CompletionCallbackWithOutput<
      typename internal::TypeUnwrapper<Output>::StorageType>
  NewCallbackWithOutput(void (T::*method)(int32_t, Output, A),
                        const A& a) {
    typedef typename internal::TypeUnwrapper<Output>::StorageType
        OutputStorageType;
    typedef CompletionCallbackWithOutput<OutputStorageType> CallbackType;
    typedef DispatcherWithOutput1<
        typename CallbackType::TraitsType,
        OutputStorageType,
        void (T::*)(int32_t, Output, A),
        typename internal::TypeUnwrapper<A>::StorageType> DispatcherType;
    return NewCallbackWithOutputHelper<CallbackType>(
        new DispatcherType(method, a));
  }

  
  
  template <typename Output, typename A>
  ext::ExtCompletionCallbackWithOutput<
      typename internal::TypeUnwrapper<Output>::StorageType>
  NewExtCallbackWithOutput(void (T::*method)(int32_t, Output, A),
                           const A& a) {
    typedef typename internal::TypeUnwrapper<Output>::StorageType
        OutputStorageType;
    typedef ext::ExtCompletionCallbackWithOutput<OutputStorageType>
        CallbackType;
    typedef DispatcherWithOutput1<
        typename CallbackType::TraitsType,
        OutputStorageType,
        void (T::*)(int32_t, Output, A),
        typename internal::TypeUnwrapper<A>::StorageType> DispatcherType;
    return NewCallbackWithOutputHelper<CallbackType>(
        new DispatcherType(method, a));
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template <typename Method, typename A, typename B>
  CompletionCallback NewCallback(Method method, const A& a, const B& b) {
    return NewCallbackHelper(new Dispatcher2<Method, A, B>(method, a, b));
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template <typename Method, typename A, typename B>
  CompletionCallback NewOptionalCallback(Method method, const A& a,
                                         const B& b) {
    CompletionCallback cc = NewCallback(method, a, b);
    cc.set_flags(cc.flags() | PP_COMPLETIONCALLBACK_FLAG_OPTIONAL);
    return cc;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template <typename Output, typename A, typename B>
  CompletionCallbackWithOutput<
      typename internal::TypeUnwrapper<Output>::StorageType>
  NewCallbackWithOutput(void (T::*method)(int32_t, Output, A, B),
                        const A& a,
                        const B& b) {
    typedef typename internal::TypeUnwrapper<Output>::StorageType
        OutputStorageType;
    typedef CompletionCallbackWithOutput<OutputStorageType> CallbackType;
    typedef DispatcherWithOutput2<
        typename CallbackType::TraitsType,
        OutputStorageType,
        void (T::*)(int32_t, Output, A, B),
        typename internal::TypeUnwrapper<A>::StorageType,
        typename internal::TypeUnwrapper<B>::StorageType> DispatcherType;
    return NewCallbackWithOutputHelper<CallbackType>(
        new DispatcherType(method, a, b));
  }

  
  
  template <typename Output, typename A, typename B>
  ext::ExtCompletionCallbackWithOutput<
      typename internal::TypeUnwrapper<Output>::StorageType>
  NewExtCallbackWithOutput(void (T::*method)(int32_t, Output, A, B),
                           const A& a,
                           const B& b) {
    typedef typename internal::TypeUnwrapper<Output>::StorageType
        OutputStorageType;
    typedef ext::ExtCompletionCallbackWithOutput<OutputStorageType>
        CallbackType;
    typedef DispatcherWithOutput2<
        typename CallbackType::TraitsType,
        OutputStorageType,
        void (T::*)(int32_t, Output, A, B),
        typename internal::TypeUnwrapper<A>::StorageType,
        typename internal::TypeUnwrapper<B>::StorageType> DispatcherType;
    return NewCallbackWithOutputHelper<CallbackType>(
        new DispatcherType(method, a, b));
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template <typename Method, typename A, typename B, typename C>
  CompletionCallback NewCallback(Method method, const A& a, const B& b,
                                 const C& c) {
    return NewCallbackHelper(new Dispatcher3<Method, A, B, C>(method, a, b, c));
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template <typename Method, typename A, typename B, typename C>
  CompletionCallback NewOptionalCallback(Method method, const A& a,
                                         const B& b, const C& c) {
    CompletionCallback cc = NewCallback(method, a, b, c);
    cc.set_flags(cc.flags() | PP_COMPLETIONCALLBACK_FLAG_OPTIONAL);
    return cc;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template <typename Output, typename A, typename B, typename C>
  CompletionCallbackWithOutput<
      typename internal::TypeUnwrapper<Output>::StorageType>
  NewCallbackWithOutput(void (T::*method)(int32_t, Output, A, B, C),
                        const A& a,
                        const B& b,
                        const C& c) {
    typedef typename internal::TypeUnwrapper<Output>::StorageType
        OutputStorageType;
    typedef CompletionCallbackWithOutput<OutputStorageType> CallbackType;
    typedef DispatcherWithOutput3<
        typename CallbackType::TraitsType,
        OutputStorageType,
        void (T::*)(int32_t, Output, A, B, C),
        typename internal::TypeUnwrapper<A>::StorageType,
        typename internal::TypeUnwrapper<B>::StorageType,
        typename internal::TypeUnwrapper<C>::StorageType> DispatcherType;
    return NewCallbackWithOutputHelper<CallbackType>(
        new DispatcherType(method, a, b, c));
  }

  
  
  template <typename Output, typename A, typename B, typename C>
  ext::ExtCompletionCallbackWithOutput<
      typename internal::TypeUnwrapper<Output>::StorageType>
  NewExtCallbackWithOutput(void (T::*method)(int32_t, Output, A, B, C),
                           const A& a,
                           const B& b,
                           const C& c) {
    typedef typename internal::TypeUnwrapper<Output>::StorageType
        OutputStorageType;
    typedef ext::ExtCompletionCallbackWithOutput<OutputStorageType>
        CallbackType;
    typedef DispatcherWithOutput3<
        typename CallbackType::TraitsType,
        OutputStorageType,
        void (T::*)(int32_t, Output, A, B, C),
        typename internal::TypeUnwrapper<A>::StorageType,
        typename internal::TypeUnwrapper<B>::StorageType,
        typename internal::TypeUnwrapper<C>::StorageType> DispatcherType;
    return NewCallbackWithOutputHelper<CallbackType>(
        new DispatcherType(method, a, b, c));
  }

 private:
  class BackPointer {
   public:
    typedef CompletionCallbackFactory<T, ThreadTraits> FactoryType;

    explicit BackPointer(FactoryType* factory)
        : factory_(factory) {
    }

    void AddRef() {
      ref_.AddRef();
    }

    void Release() {
      if (ref_.Release() == 0)
        delete this;
    }

    void DropFactory() {
      factory_ = NULL;
    }

    T* GetObject() {
      return factory_ ? factory_->GetObject() : NULL;
    }

   private:
    typename ThreadTraits::RefCount ref_;
    FactoryType* factory_;
  };

  template <typename Dispatcher>
  class CallbackData {
   public:
    
    CallbackData(BackPointer* back_pointer, Dispatcher* dispatcher)
        : back_pointer_(back_pointer),
          dispatcher_(dispatcher) {
      back_pointer_->AddRef();
    }

    ~CallbackData() {
      back_pointer_->Release();
      delete dispatcher_;
    }

    Dispatcher* dispatcher() { return dispatcher_; }

    static void Thunk(void* user_data, int32_t result) {
      Self* self = static_cast<Self*>(user_data);
      T* object = self->back_pointer_->GetObject();

      
      
      
      (*self->dispatcher_)(object, result);

      delete self;
    }

   private:
    typedef CallbackData<Dispatcher> Self;
    BackPointer* back_pointer_;  
    Dispatcher* dispatcher_;  

    
    CallbackData(const CallbackData<Dispatcher>&);
    CallbackData<Dispatcher>& operator=(const CallbackData<Dispatcher>&);
  };

  template <typename Method>
  class Dispatcher0 {
   public:
    Dispatcher0() : method_(NULL) {}
    explicit Dispatcher0(Method method) : method_(method) {
    }
    void operator()(T* object, int32_t result) {
      if (object)
        (object->*method_)(result);
    }
   private:
    Method method_;
  };

  template <typename Traits, typename Output, typename Method>
  class DispatcherWithOutput0 {
   public:
    typedef Output OutputType;

    DispatcherWithOutput0()
        : method_(NULL),
          output_() {
      Traits::Initialize(&output_);
    }
    DispatcherWithOutput0(Method method)
        : method_(method),
          output_() {
      Traits::Initialize(&output_);
    }
    void operator()(T* object, int32_t result) {
      
      
      if (object)
        (object->*method_)(result, Traits::StorageToPluginArg(output_));
      else
        Traits::StorageToPluginArg(output_);
    }
    typename Traits::StorageType* output() {
      return &output_;
    }
   private:
    Method method_;

    typename Traits::StorageType output_;
  };

  template <typename Method, typename A>
  class Dispatcher1 {
   public:
    Dispatcher1()
        : method_(NULL),
          a_() {
    }
    Dispatcher1(Method method, const A& a)
        : method_(method),
          a_(a) {
    }
    void operator()(T* object, int32_t result) {
      if (object)
        (object->*method_)(result, a_);
    }
   private:
    Method method_;
    A a_;
  };

  template <typename Traits, typename Output, typename Method, typename A>
  class DispatcherWithOutput1 {
   public:
    typedef Output OutputType;

    DispatcherWithOutput1()
        : method_(NULL),
          a_(),
          output_() {
      Traits::Initialize(&output_);
    }
    DispatcherWithOutput1(Method method, const A& a)
        : method_(method),
          a_(a),
          output_() {
      Traits::Initialize(&output_);
    }
    void operator()(T* object, int32_t result) {
      
      
      if (object)
        (object->*method_)(result, Traits::StorageToPluginArg(output_), a_);
      else
        Traits::StorageToPluginArg(output_);
    }
    typename Traits::StorageType* output() {
      return &output_;
    }
   private:
    Method method_;
    A a_;

    typename Traits::StorageType output_;
  };

  template <typename Method, typename A, typename B>
  class Dispatcher2 {
   public:
    Dispatcher2()
        : method_(NULL),
          a_(),
          b_() {
    }
    Dispatcher2(Method method, const A& a, const B& b)
        : method_(method),
          a_(a),
          b_(b) {
    }
    void operator()(T* object, int32_t result) {
      if (object)
        (object->*method_)(result, a_, b_);
    }
   private:
    Method method_;
    A a_;
    B b_;
  };

  template <typename Traits,
            typename Output,
            typename Method,
            typename A,
            typename B>
  class DispatcherWithOutput2 {
   public:
    typedef Output OutputType;

    DispatcherWithOutput2()
        : method_(NULL),
          a_(),
          b_(),
          output_() {
      Traits::Initialize(&output_);
    }
    DispatcherWithOutput2(Method method, const A& a, const B& b)
        : method_(method),
          a_(a),
          b_(b),
          output_() {
      Traits::Initialize(&output_);
    }
    void operator()(T* object, int32_t result) {
      
      
      if (object)
        (object->*method_)(result, Traits::StorageToPluginArg(output_), a_, b_);
      else
        Traits::StorageToPluginArg(output_);
    }
    typename Traits::StorageType* output() {
      return &output_;
    }
   private:
    Method method_;
    A a_;
    B b_;

    typename Traits::StorageType output_;
  };

  template <typename Method, typename A, typename B, typename C>
  class Dispatcher3 {
   public:
    Dispatcher3()
        : method_(NULL),
          a_(),
          b_(),
          c_() {
    }
    Dispatcher3(Method method, const A& a, const B& b, const C& c)
        : method_(method),
          a_(a),
          b_(b),
          c_(c) {
    }
    void operator()(T* object, int32_t result) {
      if (object)
        (object->*method_)(result, a_, b_, c_);
    }
   private:
    Method method_;
    A a_;
    B b_;
    C c_;
  };

  template <typename Traits,
            typename Output,
            typename Method,
            typename A,
            typename B,
            typename C>
  class DispatcherWithOutput3 {
   public:
    typedef Output OutputType;

    DispatcherWithOutput3()
        : method_(NULL),
          a_(),
          b_(),
          c_(),
          output_() {
      Traits::Initialize(&output_);
    }
    DispatcherWithOutput3(Method method, const A& a, const B& b, const C& c)
        : method_(method),
          a_(a),
          b_(b),
          c_(c),
          output_() {
      Traits::Initialize(&output_);
    }
    void operator()(T* object, int32_t result) {
      
      
      if (object) {
        (object->*method_)(result, Traits::StorageToPluginArg(output_),
                           a_, b_, c_);
      } else {
        Traits::StorageToPluginArg(output_);
      }
    }
    typename Traits::StorageType* output() {
      return &output_;
    }
   private:
    Method method_;
    A a_;
    B b_;
    C c_;

    typename Traits::StorageType output_;
  };

  
  
  void InitBackPointer() {
    back_pointer_ = new BackPointer(this);
    back_pointer_->AddRef();
  }

  
  
  void ResetBackPointer() {
    back_pointer_->DropFactory();
    back_pointer_->Release();
    back_pointer_ = NULL;
  }

  
  template <typename Dispatcher>
  CompletionCallback NewCallbackHelper(Dispatcher* dispatcher) {
    typename ThreadTraits::AutoLock lock(lock_);

    PP_DCHECK(object_);  
    return CompletionCallback(
        &CallbackData<Dispatcher>::Thunk,
        new CallbackData<Dispatcher>(back_pointer_, dispatcher));
  }

  
  template <typename Callback, typename Dispatcher>
  Callback NewCallbackWithOutputHelper(Dispatcher* dispatcher) {
    typename ThreadTraits::AutoLock lock(lock_);

    PP_DCHECK(object_);  
    CallbackData<Dispatcher>* data =
        new CallbackData<Dispatcher>(back_pointer_, dispatcher);

    return Callback(&CallbackData<Dispatcher>::Thunk,
                    data,
                    data->dispatcher()->output());
  }

  
  CompletionCallbackFactory(const CompletionCallbackFactory&);
  CompletionCallbackFactory& operator=(const CompletionCallbackFactory&);

  
  T* object_;

  
  typename ThreadTraits::Lock lock_;

  
  
  BackPointer* back_pointer_;
};

}  

#endif  
