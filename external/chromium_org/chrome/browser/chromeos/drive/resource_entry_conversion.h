// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_RESOURCE_ENTRY_CONVERSION_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_RESOURCE_ENTRY_CONVERSION_H_

#include <string>

namespace base {
struct PlatformFileInfo;
}

namespace google_apis {
class ResourceEntry;
}

namespace drive {

class ResourceEntry;

bool ConvertToResourceEntry(const google_apis::ResourceEntry& input,
                            ResourceEntry* out_entry,
                            std::string* out_parent_resource_id);

void ConvertResourceEntryToPlatformFileInfo(const ResourceEntry& entry,
                                            base::PlatformFileInfo* file_info);

void SetPlatformFileInfoToResourceEntry(const base::PlatformFileInfo& file_info,
                                        ResourceEntry* entry);

}  

#endif  
