// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_DIRECTORY_LISTING_PARSER_H_
#define NET_FTP_FTP_DIRECTORY_LISTING_PARSER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "net/base/net_export.h"

namespace net {

struct FtpDirectoryListingEntry {
  enum Type {
    UNKNOWN,
    FILE,
    DIRECTORY,
    SYMLINK,
  };

  FtpDirectoryListingEntry();

  Type type;
  base::string16 name;  
  std::string raw_name;  
  int64 size;  

  
  base::Time last_modified;
};

NET_EXPORT int ParseFtpDirectoryListing(
    const std::string& text,
    const base::Time& current_time,
    std::vector<FtpDirectoryListingEntry>* entries);

}  

#endif  
