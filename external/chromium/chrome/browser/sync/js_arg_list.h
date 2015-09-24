// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_JS_ARG_LIST_H_
#define CHROME_BROWSER_SYNC_JS_ARG_LIST_H_
#pragma once


#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/values.h"

namespace browser_sync {

class JsArgList {
 public:
  JsArgList();
  explicit JsArgList(const ListValue& args);
  explicit JsArgList(const std::vector<const Value*>& args);
  ~JsArgList();

  const ListValue& Get() const;

  std::string ToString() const;

 private:
  class SharedListValue : public base::RefCountedThreadSafe<SharedListValue> {
   public:
    SharedListValue();
    explicit SharedListValue(const ListValue& list_value);
    explicit SharedListValue(const std::vector<const Value*>& value_list);

    const ListValue& Get() const;

   private:
    virtual ~SharedListValue();
    friend class base::RefCountedThreadSafe<SharedListValue>;

    ListValue list_value_;
  };

  scoped_refptr<const SharedListValue> args_;

  
};

}  

#endif  
