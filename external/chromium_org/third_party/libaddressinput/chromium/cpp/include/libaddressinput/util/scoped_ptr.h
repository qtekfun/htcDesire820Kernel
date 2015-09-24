// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef I18N_ADDRESSINPUT_UTIL_SCOPED_PTR_H_
#define I18N_ADDRESSINPUT_UTIL_SCOPED_PTR_H_


#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include <algorithm>  

#include <libaddressinput/util/basictypes.h>
#include <libaddressinput/util/template_util.h>

namespace i18n {
namespace addressinput {

template <class T>
struct DefaultDeleter {
  DefaultDeleter() {}
  template <typename U> DefaultDeleter(const DefaultDeleter<U>& other) {
    
    
    
    
    
    
    
    
    
    
    
    
    enum { T_must_be_complete = sizeof(T) };
    enum { U_must_be_complete = sizeof(U) };
    COMPILE_ASSERT((is_convertible<U*, T*>::value),
                   U_ptr_must_implicitly_convert_to_T_ptr);
  }
  inline void operator()(T* ptr) const {
    enum { type_must_be_complete = sizeof(T) };
    delete ptr;
  }
};

template <class T>
struct DefaultDeleter<T[]> {
  inline void operator()(T* ptr) const {
    enum { type_must_be_complete = sizeof(T) };
    delete[] ptr;
  }

 private:
  
  
  
  
  
  
  
  template <typename U> void operator()(U* array) const;
};

template <class T, int n>
struct DefaultDeleter<T[n]> {
  
  COMPILE_ASSERT(sizeof(T) == -1, do_not_use_array_with_size_as_type);
};

struct FreeDeleter {
  inline void operator()(void* ptr) const {
    free(ptr);
  }
};

template <class T, class D>
class scoped_ptr_impl {
 public:
  explicit scoped_ptr_impl(T* p) : data_(p) { }

  
  scoped_ptr_impl(T* p, const D& d) : data_(p, d) {}

  
  
  template <typename U, typename V>
  scoped_ptr_impl(scoped_ptr_impl<U, V>* other)
      : data_(other->release(), other->get_deleter()) {
    
    
    
    
  }

  template <typename U, typename V>
  void TakeState(scoped_ptr_impl<U, V>* other) {
    
    
    reset(other->release());
    get_deleter() = other->get_deleter();
  }

  ~scoped_ptr_impl() {
    if (data_.ptr != NULL) {
      
      
      static_cast<D&>(data_)(data_.ptr);
    }
  }

  void reset(T* p) {
    
    if (p != NULL && p == data_.ptr)
      abort();

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    T* old = data_.ptr;
    data_.ptr = NULL;
    if (old != NULL)
      static_cast<D&>(data_)(old);
    data_.ptr = p;
  }

  T* get() const { return data_.ptr; }

  D& get_deleter() { return data_; }
  const D& get_deleter() const { return data_; }

  void swap(scoped_ptr_impl& p2) {
    
    
    
    using std::swap;
    swap(static_cast<D&>(data_), static_cast<D&>(p2.data_));
    swap(data_.ptr, p2.data_.ptr);
  }

  T* release() {
    T* old_ptr = data_.ptr;
    data_.ptr = NULL;
    return old_ptr;
  }

 private:
  
  template <typename U, typename V> friend class scoped_ptr_impl;

  
  
  
  
  struct Data : public D {
    explicit Data(T* ptr_in) : ptr(ptr_in) {}
    Data(T* ptr_in, const D& other) : D(other), ptr(ptr_in) {}
    T* ptr;
  };

  Data data_;

  DISALLOW_COPY_AND_ASSIGN(scoped_ptr_impl);
};

template <class T, class D = DefaultDeleter<T> >
class scoped_ptr {
 public:
  
  typedef T element_type;
  typedef D deleter_type;

  
  scoped_ptr() : impl_(NULL) { }

  
  explicit scoped_ptr(element_type* p) : impl_(p) { }

  
  scoped_ptr(element_type* p, const D& d) : impl_(p, d) { }

  
  
  
  
  
  
  
  
  
  
  template <typename U, typename V>
  scoped_ptr(scoped_ptr<U, V> other) : impl_(&other.impl_) {
    COMPILE_ASSERT(!is_array<U>::value, U_cannot_be_an_array);
  }

  
  
  
  
  
  
  
  
  
  
  template <typename U, typename V>
  scoped_ptr& operator=(scoped_ptr<U, V> rhs) {
    COMPILE_ASSERT(!is_array<U>::value, U_cannot_be_an_array);
    impl_.TakeState(&rhs.impl_);
    return *this;
  }

  
  
  void reset(element_type* p = NULL) { impl_.reset(p); }

  
  
  element_type& operator*() const {
    assert(impl_.get() != NULL);
    return *impl_.get();
  }
  element_type* operator->() const  {
    assert(impl_.get() != NULL);
    return impl_.get();
  }
  element_type* get() const { return impl_.get(); }

  
  deleter_type& get_deleter() { return impl_.get_deleter(); }
  const deleter_type& get_deleter() const { return impl_.get_deleter(); }

  
  
 private:
  typedef scoped_ptr_impl<element_type, deleter_type> scoped_ptr::*Testable;

 public:
  operator Testable() const { return impl_.get() ? &scoped_ptr::impl_ : NULL; }

  
  
  
  bool operator==(const element_type* p) const { return impl_.get() == p; }
  bool operator!=(const element_type* p) const { return impl_.get() != p; }

  
  void swap(scoped_ptr& p2) {
    impl_.swap(p2.impl_);
  }

  
  
  
  
  
  element_type* release() {
    return impl_.release();
  }

 private:
  
  template <typename U, typename V> friend class scoped_ptr;
  scoped_ptr_impl<element_type, deleter_type> impl_;

  
  
  
  
  template <class U> bool operator==(scoped_ptr<U> const& p2) const;
  template <class U> bool operator!=(scoped_ptr<U> const& p2) const;
};

template <class T, class D>
class scoped_ptr<T[], D> {
 public:
  
  typedef T element_type;
  typedef D deleter_type;

  
  scoped_ptr() : impl_(NULL) { }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  explicit scoped_ptr(element_type* array) : impl_(array) { }

  
  
  void reset(element_type* array = NULL) { impl_.reset(array); }

  
  element_type& operator[](size_t i) const {
    assert(impl_.get() != NULL);
    return impl_.get()[i];
  }
  element_type* get() const { return impl_.get(); }

  
  deleter_type& get_deleter() { return impl_.get_deleter(); }
  const deleter_type& get_deleter() const { return impl_.get_deleter(); }

  
  
 private:
  typedef scoped_ptr_impl<element_type, deleter_type> scoped_ptr::*Testable;

 public:
  operator Testable() const { return impl_.get() ? &scoped_ptr::impl_ : NULL; }

  
  
  
  bool operator==(element_type* array) const { return impl_.get() == array; }
  bool operator!=(element_type* array) const { return impl_.get() != array; }

  
  void swap(scoped_ptr& p2) {
    impl_.swap(p2.impl_);
  }

  
  
  
  
  
  element_type* release() {
    return impl_.release();
  }

 private:
  
  enum { type_must_be_complete = sizeof(element_type) };

  
  scoped_ptr_impl<element_type, deleter_type> impl_;

  
  
  
  
  
  template <typename U> explicit scoped_ptr(U* array);
  explicit scoped_ptr(int disallow_construction_from_null);

  
  
  template <typename U> void reset(U* array);
  void reset(int disallow_reset_from_null);

  
  
  
  
  template <class U> bool operator==(scoped_ptr<U> const& p2) const;
  template <class U> bool operator!=(scoped_ptr<U> const& p2) const;
};

template <class T, class D>
void swap(scoped_ptr<T, D>& p1, scoped_ptr<T, D>& p2) {
  p1.swap(p2);
}

template <class T, class D>
bool operator==(T* p1, const scoped_ptr<T, D>& p2) {
  return p1 == p2.get();
}

template <class T, class D>
bool operator!=(T* p1, const scoped_ptr<T, D>& p2) {
  return p1 != p2.get();
}

template <typename T>
scoped_ptr<T> make_scoped_ptr(T* ptr) {
  return scoped_ptr<T>(ptr);
}

}  
}  

#endif  
