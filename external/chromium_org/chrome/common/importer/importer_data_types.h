// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_IMPORTER_IMPORTER_DATA_TYPES_H_
#define CHROME_COMMON_IMPORTER_IMPORTER_DATA_TYPES_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "chrome/common/importer/importer_type.h"
#include "url/gurl.h"

namespace importer {

enum ImportItem {
  NONE           = 0,
  HISTORY        = 1 << 0,
  FAVORITES      = 1 << 1,
  COOKIES        = 1 << 2,  
  PASSWORDS      = 1 << 3,
  SEARCH_ENGINES = 1 << 4,
  HOME_PAGE      = 1 << 5,
  ALL            = (1 << 6) - 1  
};

struct SourceProfile {
  SourceProfile();
  ~SourceProfile();

  base::string16 importer_name;
  ImporterType importer_type;
  base::FilePath source_path;
  base::FilePath app_path;
  uint16 services_supported;  
  
  
  std::string locale;
};

struct URLKeywordInfo {
  GURL url;
  base::string16 keyword;
  base::string16 display_name;
};

#if defined(OS_WIN)
struct ImporterIE7PasswordInfo {
  
  std::wstring url_hash;

  
  
  std::vector<unsigned char> encrypted_data;

  
  base::Time date_created;
};
#endif

enum VisitSource {
  VISIT_SOURCE_BROWSED = 0,
  VISIT_SOURCE_FIREFOX_IMPORTED = 1,
  VISIT_SOURCE_IE_IMPORTED = 2,
  VISIT_SOURCE_SAFARI_IMPORTED = 3,
};

}  

#endif  
