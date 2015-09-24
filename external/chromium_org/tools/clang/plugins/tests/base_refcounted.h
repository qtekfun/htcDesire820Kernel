// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_REFCOUNTED_H_
#define BASE_REFCOUNTED_H_

namespace base {

template <typename T>
class RefCounted {
 public:
  RefCounted() {}
 protected:
  ~RefCounted() {}
};

template <typename T>
class RefCountedThreadSafe {
 public:
  RefCountedThreadSafe() {}
 protected:
  ~RefCountedThreadSafe() {}
};

}  

namespace WebKit {

template <typename T>
class RefCounted {
 public:
  RefCounted() {}
  ~RefCounted() {}
};

}  

class PublicRefCountedDtorInHeader
    : public base::RefCounted<PublicRefCountedDtorInHeader> {
 public:
  PublicRefCountedDtorInHeader() {}
  ~PublicRefCountedDtorInHeader() {}

 private:
  friend class base::RefCounted<PublicRefCountedDtorInHeader>;
};

class PublicRefCountedThreadSafeDtorInHeader
    : public base::RefCountedThreadSafe<
          PublicRefCountedThreadSafeDtorInHeader> {
 public:
  PublicRefCountedThreadSafeDtorInHeader() {}
  ~PublicRefCountedThreadSafeDtorInHeader() {}

 private:
  friend class base::RefCountedThreadSafe<
      PublicRefCountedThreadSafeDtorInHeader>;
};

class ProtectedRefCountedDtorInHeader
    : public base::RefCounted<ProtectedRefCountedDtorInHeader> {
 public:
  ProtectedRefCountedDtorInHeader() {}

 protected:
  ~ProtectedRefCountedDtorInHeader() {}

 private:
  friend class base::RefCounted<ProtectedRefCountedDtorInHeader>;
};

class ProtectedRefCountedVirtualDtorInHeader
    : public base::RefCounted<ProtectedRefCountedVirtualDtorInHeader> {
 public:
  ProtectedRefCountedVirtualDtorInHeader() {}

 protected:
  virtual ~ProtectedRefCountedVirtualDtorInHeader() {}

 private:
  friend class base::RefCounted<ProtectedRefCountedVirtualDtorInHeader>;
};


class PrivateRefCountedDtorInHeader
    : public base::RefCounted<PrivateRefCountedDtorInHeader> {
 public:
  PrivateRefCountedDtorInHeader() {}

 private:
  ~PrivateRefCountedDtorInHeader() {}
  friend class base::RefCounted<PrivateRefCountedDtorInHeader>;
};

class DerivedProtectedToPublicInHeader
    : public ProtectedRefCountedVirtualDtorInHeader {
 public:
  DerivedProtectedToPublicInHeader() {}
  virtual ~DerivedProtectedToPublicInHeader() {}
};

class ImplicitDerivedProtectedToPublicInHeader
    : public ProtectedRefCountedVirtualDtorInHeader {
 public:
  ImplicitDerivedProtectedToPublicInHeader() {}
};

class WebKitPublicDtorInHeader
    : public WebKit::RefCounted<WebKitPublicDtorInHeader> {
 public:
  WebKitPublicDtorInHeader() {}
  ~WebKitPublicDtorInHeader() {}
};

class WebKitDerivedPublicDtorInHeader
    : public WebKitPublicDtorInHeader {
 public:
  WebKitDerivedPublicDtorInHeader() {}
  ~WebKitDerivedPublicDtorInHeader() {}
};

class APublicInterface {
 public:
  virtual ~APublicInterface() {}
  virtual void DoFoo() = 0;
};

class ImplementsAPublicInterface
    : public APublicInterface,
      public base::RefCounted<ImplementsAPublicInterface> {
 public:
  virtual void DoFoo() override {}

 protected:
  virtual ~ImplementsAPublicInterface() {}

 private:
  friend class base::RefCounted<ImplementsAPublicInterface>;
};

class AnImplicitInterface {
 public:
  virtual void DoBar() {}
};

class ImplementsAnImplicitInterface
    : public AnImplicitInterface,
      public base::RefCounted<ImplementsAnImplicitInterface> {
 public:
  virtual void DoBar() override {}

 private:
  friend class base::RefCounted<ImplementsAnImplicitInterface>;
  ~ImplementsAnImplicitInterface() {}
};

class PrivatelyImplementsAPublicInterface
    : private APublicInterface,
      public base::RefCounted<PrivatelyImplementsAPublicInterface> {
 public:
  virtual void DoFoo() override {}

 private:
  friend class base::RefCounted<PrivatelyImplementsAPublicInterface>;
  virtual ~PrivatelyImplementsAPublicInterface() {}
};

class BaseInterface {
 public:
  virtual ~BaseInterface() {}
  virtual void DoFoo() {}
};
class DerivedInterface : public BaseInterface {
 protected:
  virtual ~DerivedInterface() {}
};
class SomeOtherInterface {
 public:
  virtual ~SomeOtherInterface() {}
  virtual void DoBar() {}
};
class RefcountedType : public base::RefCounted<RefcountedType> {
 protected:
  ~RefcountedType() {}
 private:
  friend class base::RefCounted<RefcountedType>;
};
class UnsafeInheritanceChain
    : public DerivedInterface,
      public SomeOtherInterface,
      public RefcountedType {
 public:
  
  virtual void DoFoo() override {}

  
  virtual void DoBar() override {}

 protected:
  virtual ~UnsafeInheritanceChain() {}
};

#endif  
