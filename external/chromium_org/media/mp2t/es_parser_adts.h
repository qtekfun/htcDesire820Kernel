// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP2T_ES_PARSER_ADTS_H_
#define MEDIA_MP2T_ES_PARSER_ADTS_H_

#include <list>
#include <utility>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "media/base/audio_decoder_config.h"
#include "media/base/byte_queue.h"
#include "media/mp2t/es_parser.h"

namespace media {
class AudioTimestampHelper;
class BitReader;
class StreamParserBuffer;
}

namespace media {
namespace mp2t {

class EsParserAdts : public EsParser {
 public:
  typedef base::Callback<void(const AudioDecoderConfig&)> NewAudioConfigCB;

  EsParserAdts(const NewAudioConfigCB& new_audio_config_cb,
               const EmitBufferCB& emit_buffer_cb,
               bool sbr_in_mimetype);
  virtual ~EsParserAdts();

  
  virtual bool Parse(const uint8* buf, int size,
                     base::TimeDelta pts,
                     base::TimeDelta dts) OVERRIDE;
  virtual void Flush() OVERRIDE;
  virtual void Reset() OVERRIDE;

 private:
  
  typedef std::pair<int, base::TimeDelta> EsPts;
  typedef std::list<EsPts> EsPtsList;

  
  
  
  bool UpdateAudioConfiguration(const uint8* adts_header);

  
  void DiscardEs(int nbytes);

  
  
  
  NewAudioConfigCB new_audio_config_cb_;
  EmitBufferCB emit_buffer_cb_;

  
  
  bool sbr_in_mimetype_;

  
  ByteQueue es_byte_queue_;

  
  EsPtsList pts_list_;

  
  scoped_ptr<AudioTimestampHelper> audio_timestamp_helper_;

  
  AudioDecoderConfig last_audio_decoder_config_;

  DISALLOW_COPY_AND_ASSIGN(EsParserAdts);
};

}  
}  

#endif

