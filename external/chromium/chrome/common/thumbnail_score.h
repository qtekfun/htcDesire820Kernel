// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_THUMBNAIL_SCORE_H_
#define CHROME_COMMON_THUMBNAIL_SCORE_H_
#pragma once

#include <string>
#include "base/time.h"

struct ThumbnailScore {
  
  
  
  ThumbnailScore();

  
  
  ThumbnailScore(double score, bool clipping, bool top);

  
  ThumbnailScore(double score, bool clipping, bool top,
                 const base::Time& time);
  ~ThumbnailScore();

  
  bool Equals(const ThumbnailScore& rhs) const;

  
  std::string ToString() const;

  
  
  
  
  double boring_score;

  
  
  
  
  bool good_clipping;

  
  
  
  
  
  bool at_top;

  
  
  base::Time time_at_snapshot;

  
  
  
  
  
  
  
  
  
  
  
  
  int redirect_hops_from_dest;

  
  static const double kThumbnailMaximumBoringness;

  
  
  static const double kThumbnailInterestingEnoughBoringness;

  
  
  
  static const base::TimeDelta kUpdateThumbnailTime;

  
  static const double kThumbnailDegradePerHour;

  
  
  
  bool ShouldConsiderUpdating();
};

bool ShouldReplaceThumbnailWith(const ThumbnailScore& current,
                                const ThumbnailScore& replacement);

#endif  
