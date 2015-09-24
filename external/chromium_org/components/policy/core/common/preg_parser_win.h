// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef COMPONENTS_POLICY_CORE_COMMON_PREG_PARSER_WIN_H_
#define COMPONENTS_POLICY_CORE_COMMON_PREG_PARSER_WIN_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "components/policy/policy_export.h"

namespace base {
class FilePath;
}

namespace policy {

class PolicyLoadStatusSample;
class RegistryDict;

namespace preg_parser {

POLICY_EXPORT extern const char kPRegFileHeader[8];

POLICY_EXPORT bool ReadFile(const base::FilePath& file_path,
                            const string16& root,
                            RegistryDict* dict,
                            PolicyLoadStatusSample* status);

}  
}  

#endif  
