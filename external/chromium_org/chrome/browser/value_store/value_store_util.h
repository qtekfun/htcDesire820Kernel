// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_VALUE_STORE_VALUE_STORE_UTIL_H_
#define CHROME_BROWSER_VALUE_STORE_VALUE_STORE_UTIL_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/value_store/value_store.h"

namespace value_store_util {

scoped_ptr<std::string> NewKey(const std::string& key);

scoped_ptr<std::string> NoKey();

scoped_ptr<ValueStore::Error> NoError();

}  

#endif  
