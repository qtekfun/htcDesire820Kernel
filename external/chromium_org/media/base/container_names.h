// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_CONTAINER_NAMES_H_
#define MEDIA_BASE_CONTAINER_NAMES_H_

#include "base/basictypes.h"
#include "media/base/media_export.h"

namespace media {

namespace container_names {

enum MediaContainerName {
  CONTAINER_UNKNOWN,          
  CONTAINER_AAC,              
  CONTAINER_AC3,              
  CONTAINER_AIFF,             
  CONTAINER_AMR,              
  CONTAINER_APE,              
  CONTAINER_ASF,              
  CONTAINER_ASS,              
  CONTAINER_AVI,              
  CONTAINER_BINK,             
  CONTAINER_CAF,              
  CONTAINER_DTS,              
  CONTAINER_DTSHD,            
  CONTAINER_DV,               
  CONTAINER_DXA,              
  CONTAINER_EAC3,             
  CONTAINER_FLAC,             
  CONTAINER_FLV,              
  CONTAINER_GSM,              
  CONTAINER_H261,             
  CONTAINER_H263,             
  CONTAINER_H264,             
  CONTAINER_HLS,              
  CONTAINER_IRCAM,            
  CONTAINER_MJPEG,            
  CONTAINER_MOV,              
  CONTAINER_MP3,              
  CONTAINER_MPEG2PS,          
  CONTAINER_MPEG2TS,          
  CONTAINER_MPEG4BS,          
  CONTAINER_OGG,              
  CONTAINER_RM,               
  CONTAINER_SRT,              
  CONTAINER_SWF,              
  CONTAINER_VC1,              
  CONTAINER_WAV,              
  CONTAINER_WEBM,             
  CONTAINER_WTV,              
  CONTAINER_MAX               
};

MEDIA_EXPORT MediaContainerName DetermineContainer(const uint8* buffer,
                                                   int buffer_size);

}  

}  

#endif  
