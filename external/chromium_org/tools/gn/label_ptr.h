// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_LABEL_PTR_H_
#define TOOLS_GN_LABEL_PTR_H_

#include <functional>

class ParseNode;

template<typename T>
struct LabelPtrPair {
  typedef T DestType;

  LabelPtrPair() : label(), ptr(NULL), origin(NULL) {}

  explicit LabelPtrPair(const Label& l) : label(l), ptr(NULL), origin(NULL) {
  }

  
  
  explicit LabelPtrPair(const T* p) : label(p->label()), ptr(p), origin(NULL) {
  }

  ~LabelPtrPair() {}

  Label label;
  const T* ptr;  
  const ParseNode* origin;  
};

typedef LabelPtrPair<Config> LabelConfigPair;
typedef LabelPtrPair<Target> LabelTargetPair;

typedef std::vector<LabelConfigPair> LabelConfigVector;
typedef std::vector<LabelTargetPair> LabelTargetVector;


template<typename T>
struct LabelPtrLabelEquals : public std::unary_function<Label, bool> {
  explicit LabelPtrLabelEquals(const Label& l) : label(l) {}

  bool operator()(const LabelPtrPair<T>& arg) const {
    return arg.label == label;
  }

  const Label& label;
};

template<typename T>
struct LabelPtrPtrEquals : public std::unary_function<T, bool> {
  explicit LabelPtrPtrEquals(const T* p) : ptr(p) {}

  bool operator()(const LabelPtrPair<T>& arg) const {
    return arg.ptr == ptr;
  }

  const T* ptr;
};

template<typename T>
struct LabelPtrLabelLess : public std::binary_function<LabelPtrPair<T>,
                                                       LabelPtrPair<T>,
                                                       bool> {
  bool operator()(const LabelPtrPair<T>& a, const LabelPtrPair<T>& b) const {
    return a.label < b.label;
  }
};

#endif  
