// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_VALUE_STORE_VALUE_STORE_CHANGE_H_
#define CHROME_BROWSER_VALUE_STORE_VALUE_STORE_CHANGE_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"

class ValueStoreChange;
typedef std::vector<ValueStoreChange> ValueStoreChangeList;

class ValueStoreChange {
 public:
  
  
  static std::string ToJson(const ValueStoreChangeList& changes);

  
  ValueStoreChange(
      const std::string& key, base::Value* old_value, base::Value* new_value);

  ~ValueStoreChange();

  
  const std::string& key() const;

  
  
  const base::Value* old_value() const;

  
  
  const base::Value* new_value() const;

 private:
  class Inner : public base::RefCountedThreadSafe<Inner> {
   public:
    Inner(
        const std::string& key, base::Value* old_value, base::Value* new_value);

    const std::string key_;
    const scoped_ptr<base::Value> old_value_;
    const scoped_ptr<base::Value> new_value_;

   private:
    friend class base::RefCountedThreadSafe<Inner>;
    virtual ~Inner();
  };

  scoped_refptr<Inner> inner_;
};

#endif  
