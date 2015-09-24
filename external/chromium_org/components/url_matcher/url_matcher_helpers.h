// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef COMPONENTS_URL_MATCHER_URL_MATCHER_HELPERS_H_
#define COMPONENTS_URL_MATCHER_URL_MATCHER_HELPERS_H_

#include <string>
#include <vector>

namespace base {
class Value;
}

namespace url_matcher {
namespace url_matcher_helpers {

bool GetAsStringVector(const base::Value* value, std::vector<std::string>* out);

}  
}  

#endif  
