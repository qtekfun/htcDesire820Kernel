// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_BASE_MEDIA_H_
#define MEDIA_BASE_MEDIA_H_

#include "media/base/media_export.h"

namespace base {
class FilePath;
}

namespace media {

MEDIA_EXPORT bool InitializeMediaLibrary(const base::FilePath& module_dir);

MEDIA_EXPORT void InitializeMediaLibraryForTesting();

MEDIA_EXPORT bool IsMediaLibraryInitialized();

MEDIA_EXPORT void InitializeCPUSpecificMediaFeatures();

}  

#endif  
