// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_FILE_STREAM_METRICS_H_
#define NET_BASE_FILE_STREAM_METRICS_H_

namespace net {

enum FileErrorSource {
  FILE_ERROR_SOURCE_OPEN = 0,
  FILE_ERROR_SOURCE_WRITE,
  FILE_ERROR_SOURCE_READ,
  FILE_ERROR_SOURCE_SEEK,
  FILE_ERROR_SOURCE_FLUSH,
  FILE_ERROR_SOURCE_SET_EOF,
  FILE_ERROR_SOURCE_GET_SIZE,
  FILE_ERROR_SOURCE_CLOSE,
  FILE_ERROR_SOURCE_COUNT,
};

int GetFileErrorUmaBucket(int error);

int MaxFileErrorUmaBucket();

int MaxFileErrorUmaValue();

void RecordFileError(int error, FileErrorSource source, bool record);

const char* GetFileErrorSourceName(FileErrorSource source);

}  

#endif  
