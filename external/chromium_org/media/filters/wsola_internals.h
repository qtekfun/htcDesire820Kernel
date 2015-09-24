// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_FILTERS_WSOLA_INTERNALS_H_
#define MEDIA_FILTERS_WSOLA_INTERNALS_H_

#include <utility>

#include "media/base/media_export.h"

namespace media {

class AudioBus;

namespace internal {

typedef std::pair<int, int> Interval;

MEDIA_EXPORT void MultiChannelDotProduct(const AudioBus* a,
                                         int frame_offset_a,
                                         const AudioBus* b,
                                         int frame_offset_b,
                                         int num_frames,
                                         float* dot_product);

MEDIA_EXPORT void MultiChannelMovingBlockEnergies(const AudioBus* input,
                                                  int frames_per_window,
                                                  float* energy);

MEDIA_EXPORT void CubicInterpolation(const float* y_values,
                                     float* extremum,
                                     float* extremum_value);

MEDIA_EXPORT int DecimatedSearch(int decimation,
                                 Interval exclude_interval,
                                 const AudioBus* target_block,
                                 const AudioBus* search_segment,
                                 const float* energy_target_block,
                                 const float* energy_candid_blocks);

MEDIA_EXPORT int FullSearch(int low_limit,
                            int hight_limimit,
                            Interval exclude_interval,
                            const AudioBus* target_block,
                            const AudioBus* search_block,
                            const float* energy_target_block,
                            const float* energy_candidate_blocks);

MEDIA_EXPORT int OptimalIndex(const AudioBus* search_block,
                              const AudioBus* target_block,
                              Interval exclude_interval);

MEDIA_EXPORT void GetSymmetricHanningWindow(int window_length, float* window);

}  

}  

#endif  
