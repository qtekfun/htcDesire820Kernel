// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_POLICY_PATH_PARSER_H_
#define CHROME_BROWSER_POLICY_POLICY_PATH_PARSER_H_

#include <string>

#include "base/files/file_path.h"

namespace policy {

namespace path_parser {

base::FilePath::StringType ExpandPathVariables(
    const base::FilePath::StringType& untranslated_string);

void CheckUserDataDirPolicy(base::FilePath* user_data_dir);

}  

}  

#endif  
