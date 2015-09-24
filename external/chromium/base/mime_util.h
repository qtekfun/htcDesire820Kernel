// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MIME_UTIL_H_
#define BASE_MIME_UTIL_H_
#pragma once

#include <string>

class FilePath;

namespace mime_util {

std::string GetFileMimeType(const FilePath& filepath);

std::string GetDataMimeType(const std::string& data);

#if defined(TOOLKIT_GTK)
void DetectGtkTheme();
#endif

FilePath GetMimeIcon(const std::string& mime_type, size_t size);

}  

#endif  
