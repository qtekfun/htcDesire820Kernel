// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_IMAGE_WRITER_PRIVATE_ERROR_MESSAGES_H_
#define CHROME_BROWSER_EXTENSIONS_API_IMAGE_WRITER_PRIVATE_ERROR_MESSAGES_H_

namespace extensions {
namespace image_writer {
namespace error {

extern const char kAborted[];
extern const char kNoOperationInProgress[];
extern const char kOperationAlreadyInProgress[];
extern const char kUnsupportedOperation[];

extern const char kDeviceListError[];

extern const char kInvalidFileEntry[];

extern const char kDownloadCancelled[];
extern const char kDownloadHashError[];
extern const char kDownloadInterrupted[];
extern const char kTempDirError[];
extern const char kTempFileError[];
extern const char kUrlInvalid[];

extern const char kUnzipGenericError[];
extern const char kUnzipInvalidArchive[];

extern const char kDeviceCloseError[];
extern const char kDeviceInvalid[];
extern const char kDeviceHashError[];
extern const char kDeviceOpenError[];
extern const char kDeviceWriteError[];
extern const char kImageCloseError[];
extern const char kImageInvalid[];
extern const char kImageHashError[];
extern const char kImageNotFound[];
extern const char kImageOpenError[];
extern const char kImageReadError[];
extern const char kImageSizeError[];

extern const char kHashReadError[];
extern const char kVerificationFailed[];

extern const char kChromeOSImageBurnerError[];

} 
} 
} 

#endif  
