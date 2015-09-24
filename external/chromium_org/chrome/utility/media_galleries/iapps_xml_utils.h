// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_MEDIA_GALLERIES_IAPPS_XML_UTILS_H_
#define CHROME_UTILITY_MEDIA_GALLERIES_IAPPS_XML_UTILS_H_

#include <string>

#include "base/platform_file.h"

class XmlReader;

namespace iapps {

bool SkipToNextElement(XmlReader* reader);

bool SeekToNodeAtCurrentDepth(XmlReader* reader, const std::string& name);

bool SeekInDict(XmlReader* reader, const std::string& key);

bool ReadString(XmlReader* reader, std::string* result);

bool ReadInteger(XmlReader* reader, uint64* result);

std::string ReadPlatformFileAsString(const base::PlatformFile file);

}  

#endif  
