// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_SAMPLE_FORMAT_H
#define MEDIA_BASE_SAMPLE_FORMAT_H

#include "media/base/media_export.h"

namespace media {

enum SampleFormat {
  
  
  
  kUnknownSampleFormat = 0,
  kSampleFormatU8,         
  kSampleFormatS16,        
  kSampleFormatS32,        
  kSampleFormatF32,        
  kSampleFormatPlanarS16,  
  kSampleFormatPlanarF32,  

  
  kSampleFormatMax
};

MEDIA_EXPORT int SampleFormatToBytesPerChannel(SampleFormat sample_format);

MEDIA_EXPORT const char* SampleFormatToString(SampleFormat sample_format);

}  

#endif  
