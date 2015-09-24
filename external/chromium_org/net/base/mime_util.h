// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_MIME_UTIL_H__
#define NET_BASE_MIME_UTIL_H__

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "net/base/net_export.h"

namespace net {

NET_EXPORT bool GetMimeTypeFromExtension(const base::FilePath::StringType& ext,
                                         std::string* mime_type);

NET_EXPORT bool GetWellKnownMimeTypeFromExtension(
    const base::FilePath::StringType& ext,
    std::string* mime_type);

NET_EXPORT bool GetMimeTypeFromFile(const base::FilePath& file_path,
                                    std::string* mime_type);

NET_EXPORT bool GetPreferredExtensionForMimeType(
    const std::string& mime_type,
    base::FilePath::StringType* extension);

NET_EXPORT bool IsSupportedImageMimeType(const std::string& mime_type);
NET_EXPORT bool IsSupportedMediaMimeType(const std::string& mime_type);
NET_EXPORT bool IsSupportedNonImageMimeType(const std::string& mime_type);
NET_EXPORT bool IsUnsupportedTextMimeType(const std::string& mime_type);
NET_EXPORT bool IsSupportedJavascriptMimeType(const std::string& mime_type);
NET_EXPORT bool IsSupportedCertificateMimeType(const std::string& mime_type);

NET_EXPORT bool IsSupportedMimeType(const std::string& mime_type);

NET_EXPORT bool MatchesMimeType(const std::string& mime_type_pattern,
                                const std::string& mime_type);

NET_EXPORT bool IsMimeType(const std::string& type_string);

NET_EXPORT bool AreSupportedMediaCodecs(const std::vector<std::string>& codecs);

NET_EXPORT void ParseCodecString(const std::string& codecs,
                                 std::vector<std::string>* codecs_out,
                                 bool strip);

NET_EXPORT bool IsStrictMediaMimeType(const std::string& mime_type);

NET_EXPORT bool IsSupportedStrictMediaMimeType(
    const std::string& mime_type,
    const std::vector<std::string>& codecs);

NET_EXPORT void GetExtensionsForMimeType(
    const std::string& mime_type,
    std::vector<base::FilePath::StringType>* extensions);

NET_EXPORT void RemoveProprietaryMediaTypesAndCodecsForTests();

NET_EXPORT const std::string GetIANAMediaType(const std::string& mime_type);

enum CertificateMimeType {
#define CERTIFICATE_MIME_TYPE(name, value) CERTIFICATE_MIME_TYPE_ ## name = value,
#include "net/base/mime_util_certificate_type_list.h"
#undef CERTIFICATE_MIME_TYPE
};

NET_EXPORT CertificateMimeType GetCertificateMimeTypeForMimeType(
    const std::string& mime_type);

NET_EXPORT void AddMultipartValueForUpload(const std::string& value_name,
                                           const std::string& value,
                                           const std::string& mime_boundary,
                                           const std::string& content_type,
                                           std::string* post_data);

NET_EXPORT void AddMultipartFinalDelimiterForUpload(
    const std::string& mime_boundary,
    std::string* post_data);

}  

#endif  
