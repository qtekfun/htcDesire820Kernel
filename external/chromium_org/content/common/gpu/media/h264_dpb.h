// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_H264_DPB_H_
#define CONTENT_COMMON_GPU_MEDIA_H264_DPB_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_vector.h"
#include "content/common/gpu/media/h264_parser.h"

namespace content {

struct H264Picture {
  enum Field {
    FIELD_NONE,
    FIELD_TOP,
    FIELD_BOTTOM,
  };

  
  
  
  int top_field_order_cnt;
  int bottom_field_order_cnt;
  int pic_order_cnt;
  int pic_order_cnt_msb;
  int pic_order_cnt_lsb;

  int pic_num;
  int long_term_pic_num;
  int frame_num;  
  int frame_num_offset;
  int frame_num_wrap;
  int long_term_frame_idx;

  bool idr;  
  bool ref;  
  bool long_term;  
  bool outputted;
  
  
  bool mem_mgmt_5;

  Field field;

  
  
  bool long_term_reference_flag;
  bool adaptive_ref_pic_marking_mode_flag;
  H264DecRefPicMarking ref_pic_marking[H264SliceHeader::kRefListSize];

  typedef std::vector<H264Picture*> PtrVector;
};

class H264DPB {
 public:
  H264DPB();
  ~H264DPB();

  void set_max_num_pics(size_t max_num_pics);
  size_t max_num_pics() { return max_num_pics_; }

  
  
  void DeleteUnused();

  
  void DeleteByPOC(int poc);

  
  void Clear();

  
  void StorePic(H264Picture* pic);

  
  int CountRefPics();

  
  void MarkAllUnusedForRef();

  
  H264Picture* GetShortRefPicByPicNum(int pic_num);

  
  H264Picture* GetLongRefPicByLongTermPicNum(int pic_num);

  
  
  H264Picture* GetLowestFrameNumWrapShortRefPic();

  
  
  void GetNotOutputtedPicsAppending(H264Picture::PtrVector& out);

  
  void GetShortTermRefPicsAppending(H264Picture::PtrVector& out);

  
  void GetLongTermRefPicsAppending(H264Picture::PtrVector& out);

  
  
  typedef ScopedVector<H264Picture> Pictures;
  Pictures::iterator begin() { return pics_.begin(); }
  Pictures::iterator end() { return pics_.end(); }
  Pictures::reverse_iterator rbegin() { return pics_.rbegin(); }
  Pictures::reverse_iterator rend() { return pics_.rend(); }

  size_t size() const { return pics_.size(); }
  bool IsFull() const { return pics_.size() == max_num_pics_; }

  
  enum { kDPBMaxSize = 16, };

 private:
  Pictures pics_;
  size_t max_num_pics_;

  DISALLOW_COPY_AND_ASSIGN(H264DPB);
};

}  

#endif  
