// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_UTIL_MAC_H_
#define CHROME_BROWSER_UI_COCOA_DOWNLOAD_DOWNLOAD_UTIL_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

class FilePath;

namespace download_util {

void AddFileToPasteboard(NSPasteboard* pasteboard, const FilePath& path);

void NotifySystemOfDownloadComplete(const FilePath& path);

}  

#endif  
