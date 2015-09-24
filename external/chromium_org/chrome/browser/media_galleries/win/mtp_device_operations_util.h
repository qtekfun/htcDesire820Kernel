// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_MEDIA_GALLERIES_WIN_MTP_DEVICE_OPERATIONS_UTIL_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_WIN_MTP_DEVICE_OPERATIONS_UTIL_H_

#include <portabledeviceapi.h>

#include <string>

#include "base/platform_file.h"
#include "base/strings/string16.h"
#include "base/win/scoped_comptr.h"
#include "chrome/browser/media_galleries/win/mtp_device_object_entry.h"

namespace media_transfer_protocol {

base::win::ScopedComPtr<IPortableDevice> OpenDevice(
    const base::string16& pnp_device_id);

base::PlatformFileError GetFileEntryInfo(
    IPortableDevice* device,
    const base::string16& object_id,
    base::PlatformFileInfo* file_entry_info);

bool GetDirectoryEntries(IPortableDevice* device,
                         const base::string16& directory_object_id,
                         MTPDeviceObjectEntries* object_entries);

HRESULT GetFileStreamForObject(IPortableDevice* device,
                               const base::string16& file_object_id,
                               IStream** file_stream,
                               DWORD* optimal_transfer_size);

// value indicating the total number of bytes written to the file specified
DWORD CopyDataChunkToLocalFile(IStream* stream,
                               const base::FilePath& local_path,
                               size_t optimal_transfer_size);

base::string16 GetObjectIdFromName(IPortableDevice* device,
                                   const base::string16& parent_id,
                                   const base::string16& object_name);

}  

#endif  
