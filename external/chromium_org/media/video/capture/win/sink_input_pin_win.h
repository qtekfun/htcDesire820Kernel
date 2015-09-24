// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_VIDEO_CAPTURE_WIN_SINK_INPUT_PIN_WIN_H_
#define MEDIA_VIDEO_CAPTURE_WIN_SINK_INPUT_PIN_WIN_H_

#include "media/video/capture/video_capture_device.h"
#include "media/video/capture/video_capture_types.h"
#include "media/video/capture/win/pin_base_win.h"
#include "media/video/capture/win/sink_filter_win.h"

namespace media {

extern const REFERENCE_TIME kSecondsToReferenceTime;

class SinkInputPin : public PinBase {
 public:
  SinkInputPin(IBaseFilter* filter, SinkFilterObserver* observer);
  virtual ~SinkInputPin();

  void SetRequestedMediaFormat(const VideoCaptureFormat& format);
  
  
  const VideoCaptureFormat& ResultingFormat();

  
  virtual bool IsMediaTypeValid(const AM_MEDIA_TYPE* media_type);
  virtual bool GetValidMediaType(int index, AM_MEDIA_TYPE* media_type);

  STDMETHOD(Receive)(IMediaSample* media_sample);

 private:
  VideoCaptureFormat requested_format_;
  VideoCaptureFormat resulting_format_;
  SinkFilterObserver* observer_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(SinkInputPin);
};

}  

#endif  
