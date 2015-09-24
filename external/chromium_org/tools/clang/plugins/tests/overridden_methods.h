// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OVERRIDDEN_METHODS_H_
#define OVERRIDDEN_METHODS_H_

class BaseClass {
 public:
  virtual ~BaseClass() {}
  virtual void SomeMethod() = 0;
  virtual void SomeOtherMethod() = 0;
  virtual void SomeInlineMethod() = 0;
  virtual void SomeConstMethod() const = 0;
  virtual void SomeMethodWithExceptionSpec() throw() = 0;
  virtual void SomeConstMethodWithExceptionSpec() const throw(int) = 0;
  virtual void SomeNonPureBaseMethod() {}
  virtual void SomeMethodWithComment() = 0;
  virtual void SomeMethodWithCommentAndBody() = 0;
};

class InterimClass : public BaseClass {
  
  virtual void SomeMethod() = 0;
};

namespace WebKit {
class WebKitObserver {
 public:
  virtual void WebKitModifiedSomething() {};
};
}  

namespace webkit_glue {
class WebKitObserverImpl : WebKit::WebKitObserver {
 public:
  virtual void WebKitModifiedSomething() {};
};
}  

class DerivedClass : public InterimClass,
                     public webkit_glue::WebKitObserverImpl {
 public:
  
  virtual ~DerivedClass() {}
  
  virtual void SomeMethod();
  
  virtual void SomeOtherMethod() override;
  
  virtual void SomeInlineMethod() {}
  
  virtual void WebKitModifiedSomething();
  
  virtual void SomeConstMethod() const {}
  
  virtual void SomeMethodWithExceptionSpec() throw() {}
  
  
  virtual void SomeConstMethodWithExceptionSpec() const throw(int) {}
  
  virtual void SomeNonPureBaseMethod() {}
  
  virtual void SomeMethodWithComment();  
  
  virtual void SomeMethodWithCommentAndBody() {}  
};

#endif  
