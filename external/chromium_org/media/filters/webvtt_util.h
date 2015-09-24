// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_WEBVTT_UTIL_H_
#define MEDIA_FILTERS_WEBVTT_UTIL_H_

#include <vector>

namespace media {

template<typename T>
void MakeSideData(T id_begin, T id_end,
                  T settings_begin, T settings_end,
                  std::vector<uint8>* side_data) {
  
  
  
  
  side_data->clear();
  side_data->insert(side_data->end(), id_begin, id_end);
  side_data->push_back(0);
  side_data->insert(side_data->end(), settings_begin, settings_end);
  side_data->push_back(0);
}

}  

#endif  
