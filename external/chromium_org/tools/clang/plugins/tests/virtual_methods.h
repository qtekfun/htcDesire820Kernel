// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef VIRTUAL_METHODS_H_
#define VIRTUAL_METHODS_H_

class VirtualMethodsInHeaders {
 public:
  
  virtual void MethodIsAbstract() = 0;
  virtual void MethodHasNoArguments();
  virtual void MethodHasEmptyDefaultImpl() {}

  
  virtual bool ComplainAboutThis() { return true; }
};

class WarnOnMissingVirtual : public VirtualMethodsInHeaders {
 public:
  void MethodHasNoArguments() override;
};

namespace testing {
struct TestStruct {};
}  

class VirtualMethodsInHeadersTesting : public VirtualMethodsInHeaders {
 public:
  
  void MethodHasNoArguments();
 private:
  testing::TestStruct tester_;
};

#endif  
