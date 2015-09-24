// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_SEARCH_METADATA_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_SEARCH_METADATA_H_

#include <string>

#include "chrome/browser/chromeos/drive/file_system_interface.h"

namespace base {
namespace i18n {
class FixedPatternStringSearchIgnoringCaseAndAccents;
}  
}  

namespace drive {
namespace internal {

class ResourceMetadata;

void SearchMetadata(
    scoped_refptr<base::SequencedTaskRunner> blocking_task_runner,
    ResourceMetadata* resource_metadata,
    const std::string& query,
    int search_options,
    int at_most_num_matches,
    const SearchMetadataCallback& callback);

bool FindAndHighlight(
    const std::string& text,
    base::i18n::FixedPatternStringSearchIgnoringCaseAndAccents* query,
    std::string* highlighted_text);

}  
}  

#endif  
