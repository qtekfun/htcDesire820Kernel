// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_DIRECTORY_LISTING_PARSER_WINDOWS_H_
#define NET_FTP_FTP_DIRECTORY_LISTING_PARSER_WINDOWS_H_

#include <vector>

#include "base/strings/string16.h"
#include "net/base/net_export.h"

namespace net {

struct FtpDirectoryListingEntry;

NET_EXPORT_PRIVATE bool ParseFtpDirectoryListingWindows(
    const std::vector<base::string16>& lines,
    std::vector<FtpDirectoryListingEntry>* entries);

}  

#endif  
