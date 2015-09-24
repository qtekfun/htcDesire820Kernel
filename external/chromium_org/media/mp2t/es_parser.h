// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP2T_ES_PARSER_H_
#define MEDIA_MP2T_ES_PARSER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"

namespace media {

class StreamParserBuffer;

namespace mp2t {

class EsParser {
 public:
  typedef base::Callback<void(scoped_refptr<StreamParserBuffer>)> EmitBufferCB;

  EsParser() {}
  virtual ~EsParser() {}

  
  
  virtual bool Parse(const uint8* buf, int size,
                     base::TimeDelta pts,
                     base::TimeDelta dts) = 0;

  
  virtual void Flush() = 0;

  
  virtual void Reset() = 0;
};

}  
}  

#endif
