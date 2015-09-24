// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GIN_PER_CONTEXT_DATA_H_
#define GIN_PER_CONTEXT_DATA_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "gin/gin_export.h"
#include "v8/include/v8.h"

namespace gin {

class Runner;

class GIN_EXPORT ContextSupplement {
 public:
  ContextSupplement();
  virtual ~ContextSupplement();

  
  
  virtual void Detach(v8::Handle<v8::Context> context) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ContextSupplement);
};

class GIN_EXPORT PerContextData {
 public:
  explicit PerContextData(v8::Handle<v8::Context> context);
  ~PerContextData();

  
  static PerContextData* From(v8::Handle<v8::Context>);
  void Detach(v8::Handle<v8::Context> context);

  
  
  Runner* runner() const { return runner_; }
  void set_runner(Runner* runner) { runner_ = runner; }

  void AddSupplement(scoped_ptr<ContextSupplement> supplement);

 private:
  typedef ScopedVector<ContextSupplement> SuplementVector;

  Runner* runner_;
  SuplementVector supplements_;

  DISALLOW_COPY_AND_ASSIGN(PerContextData);
};

}  

#endif  
