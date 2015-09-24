// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIDEVINE_CDM_WIDEVINE_CDM_COMMON_H_
#define WIDEVINE_CDM_WIDEVINE_CDM_COMMON_H_

#include "base/files/file_path.h"


const int kWidevineCdmVersionNumComponents = 4;

const char kWidevineKeySystem[] = "com.widevine.alpha";

const char kWidevineCdmDisplayName[] = "Widevine Content Decryption Module";
const char kWidevineCdmDescription[] =
    "Enables Widevine licenses for playback of HTML audio/video content.";

#if defined(ENABLE_PEPPER_CDMS)
const char kWidevineCdmPluginMimeType[] = "application/x-ppapi-widevine-cdm";
const char kWidevineCdmPluginMimeTypeDescription[] =
    "Widevine Content Decryption Module";

const char kWidevineCdmFileName[] =
#if defined(OS_MACOSX)
    "libwidevinecdm.dylib";
#elif defined(OS_WIN)
    "widevinecdm.dll";
#else  
    "libwidevinecdm.so";
#endif

const char kWidevineCdmAdapterFileName[] =
#if defined(OS_MACOSX)
    "widevinecdmadapter.plugin";
#elif defined(OS_WIN)
    "widevinecdmadapter.dll";
#else  
    "libwidevinecdmadapter.so";
#endif

const char kCdmSupportedCodecsParamName[] = "codecs";
const char kCdmSupportedCodecsValueDelimiter = ',';
const char kCdmSupportedCodecVorbis[] = "vorbis";
const char kCdmSupportedCodecVp8[] = "vp8";
#if defined(USE_PROPRIETARY_CODECS)
const char kCdmSupportedCodecAac[] = "aac";
const char kCdmSupportedCodecAvc1[] = "avc1";
#endif  

#if defined(OS_MACOSX) || defined(OS_WIN)
#define WIDEVINE_CDM_IS_COMPONENT
#endif  
#endif  

#endif  
