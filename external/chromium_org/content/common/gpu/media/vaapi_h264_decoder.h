// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_VAAPI_H264_DECODER_H_
#define CONTENT_COMMON_GPU_MEDIA_VAAPI_H264_DECODER_H_

#include <vector>

#include "base/callback_forward.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/gpu/media/h264_dpb.h"
#include "content/common/gpu/media/h264_parser.h"
#include "content/common/gpu/media/vaapi_wrapper.h"
#include "media/base/limits.h"

namespace content {

class VaapiH264Decoder {
 public:
  
  
  
  typedef base::Callback<
      void(int32, const scoped_refptr<VASurface>&)> OutputPicCB;

  enum VAVDAH264DecoderFailure {
    FRAME_MBS_ONLY_FLAG_NOT_ONE = 0,
    GAPS_IN_FRAME_NUM = 1,
    MID_STREAM_RESOLUTION_CHANGE = 2,
    INTERLACED_STREAM = 3,
    VAAPI_ERROR = 4,
    VAVDA_H264_DECODER_FAILURES_MAX,
  };

  
  
  typedef base::Callback<void(VAVDAH264DecoderFailure error)>
      ReportErrorToUmaCB;

  
  enum DecResult {
    kDecodeError,  
    
    
    
    
    kAllocateNewSurfaces,  
    kRanOutOfStreamData,  
    kRanOutOfSurfaces,  
  };

  
  
  
  
  VaapiH264Decoder(VaapiWrapper* vaapi_wrapper,
                   const OutputPicCB& output_pic_cb,
                   const ReportErrorToUmaCB& report_error_to_uma_cb);

  ~VaapiH264Decoder();

  
  
  bool Flush() WARN_UNUSED_RESULT;

  
  
  
  
  void Reset();

  
  
  
  void SetStream(uint8* ptr, size_t size, int32 input_id);

  
  
  
  DecResult Decode() WARN_UNUSED_RESULT;

  
  
  
  gfx::Size GetPicSize() { return pic_size_; }
  size_t GetRequiredNumOfPictures();

  
  void ReuseSurface(const scoped_refptr<VASurface>& va_surface);

 private:
  
  
  
  
  
  
  enum {
    kPicsInPipeline = media::limits::kMaxVideoFrames + 2,
    kMaxNumReqPictures = H264DPB::kDPBMaxSize + kPicsInPipeline,
  };

  
  enum State {
    kNeedStreamMetadata,  
    kDecoding,  
    kAfterReset, 
    kError,  
  };

  
  bool ProcessSPS(int sps_id, bool* need_new_buffers);
  bool ProcessPPS(int pps_id);
  bool ProcessSlice(H264SliceHeader* slice_hdr);

  
  bool InitCurrPicture(H264SliceHeader* slice_hdr);

  
  
  bool CalculatePicOrderCounts(H264SliceHeader* slice_hdr);

  
  
  void UpdatePicNums();

  
  bool PrepareRefPicLists(H264SliceHeader* slice_hdr);

  
  
  void ConstructReferencePicListsP(H264SliceHeader* slice_hdr);
  void ConstructReferencePicListsB(H264SliceHeader* slice_hdr);

  
  int PicNumF(H264Picture *pic);
  int LongTermPicNumF(H264Picture *pic);

  
  
  
  
  bool ModifyReferencePicList(H264SliceHeader *slice_hdr, int list);

  
  
  
  bool HandleMemoryManagementOps();
  void ReferencePictureMarking();

  
  bool StartNewFrame(H264SliceHeader* slice_hdr);

  
  bool FinishPrevFrameIfPresent();

  
  
  
  
  bool FinishPicture();

  
  
  
  void ClearDPB();

  
  bool SendPPS();
  bool SendIQMatrix();
  bool SendVASliceParam(H264SliceHeader* slice_hdr);
  bool SendSliceData(const uint8* ptr, size_t size);
  bool QueueSlice(H264SliceHeader* slice_hdr);

  
  void FillVAPicture(VAPictureH264 *va_pic, H264Picture* pic);
  int FillVARefFramesFromDPB(VAPictureH264 *va_pics, int num_pics);

  
  bool DecodePicture();

  
  bool OutputPic(H264Picture* pic);

  
  bool OutputAllRemainingPics();

  
  
  class DecodeSurface;

  
  
  
  bool AssignSurfaceToPoC(int32 input_id, int poc);

  
  void UnassignSurfaceFromPoC(int poc);

  
  DecodeSurface* DecodeSurfaceByPoC(int poc);

  
  State state_;

  
  H264Parser parser_;

  
  H264DPB dpb_;

  
  scoped_ptr<H264Picture> curr_pic_;

  
  
  H264Picture::PtrVector ref_pic_list0_;
  H264Picture::PtrVector ref_pic_list1_;

  
  int max_pic_order_cnt_lsb_;
  int max_frame_num_;
  int max_pic_num_;
  int max_long_term_frame_idx_;

  int frame_num_;
  int prev_frame_num_;
  int prev_frame_num_offset_;
  bool prev_has_memmgmnt5_;

  
  bool prev_ref_has_memmgmnt5_;
  int prev_ref_top_field_order_cnt_;
  int prev_ref_pic_order_cnt_msb_;
  int prev_ref_pic_order_cnt_lsb_;
  H264Picture::Field prev_ref_field_;

  
  int curr_sps_id_;
  int curr_pps_id_;

  
  gfx::Size pic_size_;

  
  typedef std::map<int, linked_ptr<DecodeSurface> > DecSurfacesInUse;
  DecSurfacesInUse decode_surfaces_in_use_;

  
  std::vector<scoped_refptr<VASurface> > available_va_surfaces_;

  
  
  int32 curr_input_id_;

  VaapiWrapper* vaapi_wrapper_;

  
  OutputPicCB output_pic_cb_;

  
  
  ReportErrorToUmaCB report_error_to_uma_cb_;

  
  int last_output_poc_;

  
  int max_dpb_size_;

  DISALLOW_COPY_AND_ASSIGN(VaapiH264Decoder);
};

}  

#endif  
