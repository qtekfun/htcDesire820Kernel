// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_BROWSER_NACL_FILE_HOST_H_
#define COMPONENTS_NACL_BROWSER_NACL_FILE_HOST_H_

#include <string>

#include "base/memory/ref_counted.h"

class GURL;

namespace base {
class FilePath;
}

namespace IPC {
class Message;
}

namespace nacl {
class NaClHostMessageFilter;
}


namespace nacl_file_host {

void GetReadonlyPnaclFd(
    scoped_refptr<nacl::NaClHostMessageFilter> nacl_host_message_filter,
    const std::string& filename,
    IPC::Message* reply_msg);

bool PnaclCanOpenFile(const std::string& filename,
                      base::FilePath* file_to_open);

void OpenNaClExecutable(
    scoped_refptr<nacl::NaClHostMessageFilter> nacl_host_message_filter,
    int render_view_id,
    const GURL& file_url,
    IPC::Message* reply_msg);

}  

#endif  
