// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_MEDIA_CAST_RTP_STREAM_H_
#define CHROME_RENDERER_MEDIA_CAST_RTP_STREAM_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/WebKit/public/platform/WebMediaStreamTrack.h"

class CastAudioSink;
class CastSession;
class CastVideoSink;

struct CastCodecSpecificParams {
  std::string key;
  std::string value;

  CastCodecSpecificParams();
  ~CastCodecSpecificParams();
};

struct CastRtpPayloadParams {
  
  int payload_type;

  
  int ssrc;

  
  int clock_rate;

  
  int max_bitrate;

  
  int min_bitrate;

  
  int channels;

  
  int width;
  int height;

  
  std::string codec_name;

  
  std::vector<CastCodecSpecificParams> codec_specific_params;

  CastRtpPayloadParams();
  ~CastRtpPayloadParams();
};

struct CastRtpCaps {
  
  std::vector<CastRtpPayloadParams> payloads;

  
  std::vector<std::string> rtcp_features;

  
  std::vector<std::string> fec_mechanisms;

  CastRtpCaps();
  ~CastRtpCaps();
};

typedef CastRtpCaps CastRtpParams;

class CastRtpStream {
 public:
  CastRtpStream(const blink::WebMediaStreamTrack& track,
                const scoped_refptr<CastSession>& session);
  ~CastRtpStream();

  
  CastRtpCaps GetCaps();

  
  CastRtpParams GetParams();

  
  
  void Start(const CastRtpParams& params);

  
  void Stop();

 private:
  
  
  bool IsAudio() const;

  blink::WebMediaStreamTrack track_;
  const scoped_refptr<CastSession> cast_session_;
  scoped_ptr<CastAudioSink> audio_sink_;
  scoped_ptr<CastVideoSink> video_sink_;
  CastRtpParams params_;

  DISALLOW_COPY_AND_ASSIGN(CastRtpStream);
};

#endif  
