// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_CHUNK_RANGE_H_
#define CHROME_BROWSER_SAFE_BROWSING_CHUNK_RANGE_H_

#include <string>
#include <vector>


class ChunkRange {
 public:
  explicit ChunkRange(int start);
  ChunkRange(int start, int stop);
  ChunkRange(const ChunkRange& rhs);

  inline int start() const { return start_; }
  inline int stop() const { return stop_; }

  bool operator==(const ChunkRange& rhs) const {
    return start_ == rhs.start() && stop_ == rhs.stop();
  }

 private:
  int start_;
  int stop_;
};



void RangesToChunks(const std::vector<ChunkRange>& ranges,
                    std::vector<int>* chunks);

bool StringToRanges(const std::string& input,
                    std::vector<ChunkRange>* ranges);

void ChunksToRangeString(const std::vector<int>& chunks, std::string* result);

bool IsChunkInRange(int chunk_number, const std::vector<ChunkRange>& ranges);

#endif  
