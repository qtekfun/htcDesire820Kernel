// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_UTIL_H_

#include "base/callback_forward.h"
#include "google_apis/drive/gdata_wapi_parser.h"
#include "url/gurl.h"

class ExtensionFunctionDispatcher;
class Profile;

namespace content {
class RenderViewHost;
class WebContents;
}

namespace extensions {
namespace api {
namespace file_browser_private {
struct VolumeMetadata;
}
}
}

namespace ui {
struct SelectedFileInfo;
}

namespace file_manager {

struct VolumeInfo;

namespace util {

void VolumeInfoToVolumeMetadata(
    Profile* profile,
    const VolumeInfo& volume_info,
    extensions::api::file_browser_private::VolumeMetadata* volume_metadata);

content::WebContents* GetWebContents(ExtensionFunctionDispatcher* dispatcher);

base::FilePath GetLocalPathFromURL(
    content::RenderViewHost* render_view_host,
    Profile* profile,
    const GURL& url);

typedef base::Callback<void(const std::vector<ui::SelectedFileInfo>&)>
    GetSelectedFileInfoCallback;

enum GetSelectedFileInfoLocalPathOption {
  NO_LOCAL_PATH_RESOLUTION,
  NEED_LOCAL_PATH_FOR_OPENING,
  NEED_LOCAL_PATH_FOR_SAVING,
};

void GetSelectedFileInfo(content::RenderViewHost* render_view_host,
                         Profile* profile,
                         const std::vector<GURL>& file_urls,
                         GetSelectedFileInfoLocalPathOption local_path_option,
                         GetSelectedFileInfoCallback callback);

}  
}  

#endif  
