// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COCOA_FILE_METADATA_H_
#define CHROME_BROWSER_COCOA_FILE_METADATA_H_
#pragma once

class FilePath;
class GURL;

namespace file_metadata {

void AddOriginMetadataToFile(const FilePath& file, const GURL& source,
                             const GURL& referrer);

void AddQuarantineMetadataToFile(const FilePath& file, const GURL& source,
                                 const GURL& referrer);

}  

#endif  
