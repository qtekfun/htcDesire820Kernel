// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_TEST_COMPLETION_CALLBACK_H_
#define NET_BASE_TEST_COMPLETION_CALLBACK_H_

#include "base/compiler_specific.h"
#include "base/tuple.h"
#include "net/base/completion_callback.h"
#include "net/base/net_errors.h"



namespace net {

class IOBuffer;

namespace internal {

class TestCompletionCallbackBaseInternal {
 public:
  bool have_result() const { return have_result_; }

 protected:
  TestCompletionCallbackBaseInternal();
  void DidSetResult();
  void WaitForResult();

  bool have_result_;
  bool waiting_for_result_;

 private:
  DISALLOW_COPY_AND_ASSIGN(TestCompletionCallbackBaseInternal);
};

template <typename R>
class TestCompletionCallbackTemplate
    : public TestCompletionCallbackBaseInternal {
 public:
  virtual ~TestCompletionCallbackTemplate() {}

  R WaitForResult() {
    TestCompletionCallbackBaseInternal::WaitForResult();
    return result_;
  }

  R GetResult(R result) {
    if (net::ERR_IO_PENDING != result)
      return result;
    return WaitForResult();
  }

 protected:
  
  virtual void SetResult(R result) {
    result_ = result;
    DidSetResult();
  }

  TestCompletionCallbackTemplate() : result_(R()) {}
  R result_;

 private:
  DISALLOW_COPY_AND_ASSIGN(TestCompletionCallbackTemplate);
};

}  

typedef internal::TestCompletionCallbackTemplate<int>
    TestCompletionCallbackBase;

typedef internal::TestCompletionCallbackTemplate<int64>
    TestInt64CompletionCallbackBase;

class TestCompletionCallback : public TestCompletionCallbackBase {
 public:
  TestCompletionCallback();
  virtual ~TestCompletionCallback();

  const CompletionCallback& callback() const { return callback_; }

 private:
  const CompletionCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(TestCompletionCallback);
};

class TestInt64CompletionCallback : public TestInt64CompletionCallbackBase {
 public:
  TestInt64CompletionCallback();
  virtual ~TestInt64CompletionCallback();

  const Int64CompletionCallback& callback() const { return callback_; }

 private:
  const Int64CompletionCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(TestInt64CompletionCallback);
};

class ReleaseBufferCompletionCallback: public TestCompletionCallback {
 public:
  explicit ReleaseBufferCompletionCallback(IOBuffer* buffer);
  virtual ~ReleaseBufferCompletionCallback();

 private:
  virtual void SetResult(int result) OVERRIDE;

  IOBuffer* buffer_;
  DISALLOW_COPY_AND_ASSIGN(ReleaseBufferCompletionCallback);
};

}  

#endif  
