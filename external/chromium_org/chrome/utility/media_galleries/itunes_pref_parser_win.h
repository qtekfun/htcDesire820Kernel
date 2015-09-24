// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_MEDIA_GALLERIES_ITUNES_PREF_PARSER_WIN_H_
#define CHROME_UTILITY_MEDIA_GALLERIES_ITUNES_PREF_PARSER_WIN_H_

#include <string>

#include "base/files/file_path.h"

namespace itunes {

base::FilePath::StringType FindLibraryLocationInPrefXml(
    const std::string& pref_xml_data);

}  

#endif  
