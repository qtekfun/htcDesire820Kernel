// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_URL_REQUEST_JOB_FACTORY_H_
#define WEBKIT_BROWSER_FILEAPI_FILE_SYSTEM_URL_REQUEST_JOB_FACTORY_H_

#include "net/url_request/url_request_job_factory.h"

#include "webkit/browser/webkit_storage_browser_export.h"

namespace base {
class MessageLoopProxy;
}  

namespace fileapi {

class FileSystemContext;

WEBKIT_STORAGE_BROWSER_EXPORT net::URLRequestJobFactory::ProtocolHandler*
    CreateFileSystemProtocolHandler(FileSystemContext* context);

}  

#endif  
