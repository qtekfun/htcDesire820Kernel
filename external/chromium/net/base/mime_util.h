// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_MIME_UTIL_H__
#define NET_BASE_MIME_UTIL_H__
#pragma once

#include <string>
#include <vector>

#include "base/file_path.h"
#include "net/base/net_export.h"

namespace net {

bool GetMimeTypeFromExtension(const FilePath::StringType& ext,
                              std::string* mime_type);

NET_EXPORT bool GetMimeTypeFromFile(const FilePath& file_path, std::string* mime_type);

bool GetPreferredExtensionForMimeType(const std::string& mime_type,
                                      FilePath::StringType* extension);

bool IsSupportedImageMimeType(const char* mime_type);
bool IsSupportedMediaMimeType(const char* mime_type);
bool IsSupportedNonImageMimeType(const char* mime_type);
bool IsSupportedJavascriptMimeType(const char* mime_type);

bool IsViewSourceMimeType(const char* mime_type);

bool IsSupportedMimeType(const std::string& mime_type);

bool MatchesMimeType(const std::string &mime_type_pattern,
                     const std::string &mime_type);

bool AreSupportedMediaCodecs(const std::vector<std::string>& codecs);

void ParseCodecString(const std::string& codecs,
                      std::vector<std::string>* codecs_out,
                      bool strip);

bool IsStrictMediaMimeType(const std::string& mime_type);

bool IsSupportedStrictMediaMimeType(const std::string& mime_type,
    const std::vector<std::string>& codecs);

void GetImageExtensions(std::vector<FilePath::StringType>* extensions);

void GetAudioExtensions(std::vector<FilePath::StringType>* extensions);

void GetVideoExtensions(std::vector<FilePath::StringType>* extensions);

void GetExtensionsForMimeType(const std::string& mime_type,
                              std::vector<FilePath::StringType>* extensions);

}  

#endif  
