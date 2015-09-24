// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WORKSPACE_MAGNETISM_MATCHER_H_
#define ASH_WM_WORKSPACE_MAGNETISM_MATCHER_H_

#include <utility>
#include <vector>

#include "ash/ash_export.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/memory/scoped_vector.h"
#include "ui/gfx/rect.h"

namespace ash {
namespace internal {

enum MagnetismEdge {
  MAGNETISM_EDGE_TOP    = 1 << 0,
  MAGNETISM_EDGE_LEFT   = 1 << 1,
  MAGNETISM_EDGE_BOTTOM = 1 << 2,
  MAGNETISM_EDGE_RIGHT  = 1 << 3,
};

const uint32 kAllMagnetismEdges =
    MAGNETISM_EDGE_TOP | MAGNETISM_EDGE_LEFT | MAGNETISM_EDGE_BOTTOM |
    MAGNETISM_EDGE_RIGHT;

class MagnetismEdgeMatcher {
 public:
  MagnetismEdgeMatcher(const gfx::Rect& bounds, MagnetismEdge edge);
  ~MagnetismEdgeMatcher();

  MagnetismEdge edge() const { return edge_; }
  const gfx::Rect& bounds() const { return bounds_; }

  
  
  bool is_edge_obscured() const { return ranges_.empty(); }

  
  bool ShouldAttach(const gfx::Rect& bounds);

 private:
  typedef std::pair<int,int> Range;
  typedef std::vector<Range> Ranges;

  
  void UpdateRanges(const Range& range);

  static int GetPrimaryCoordinate(const gfx::Rect& bounds, MagnetismEdge edge) {
    switch (edge) {
      case MAGNETISM_EDGE_TOP:
        return bounds.y();
      case MAGNETISM_EDGE_LEFT:
        return bounds.x();
      case MAGNETISM_EDGE_BOTTOM:
        return bounds.bottom();
      case MAGNETISM_EDGE_RIGHT:
        return bounds.right();
    }
    NOTREACHED();
    return 0;
  }

  static MagnetismEdge FlipEdge(MagnetismEdge edge) {
    switch (edge) {
      case MAGNETISM_EDGE_TOP:
        return MAGNETISM_EDGE_BOTTOM;
      case MAGNETISM_EDGE_BOTTOM:
        return MAGNETISM_EDGE_TOP;
      case MAGNETISM_EDGE_LEFT:
        return MAGNETISM_EDGE_RIGHT;
      case MAGNETISM_EDGE_RIGHT:
        return MAGNETISM_EDGE_LEFT;
    }
    NOTREACHED();
    return MAGNETISM_EDGE_LEFT;
  }

  Range GetPrimaryRange(const gfx::Rect& bounds) const {
    switch (edge_) {
      case MAGNETISM_EDGE_TOP:
      case MAGNETISM_EDGE_BOTTOM:
        return Range(bounds.y(), bounds.bottom());
      case MAGNETISM_EDGE_LEFT:
      case MAGNETISM_EDGE_RIGHT:
        return Range(bounds.x(), bounds.right());
    }
    NOTREACHED();
    return Range();
  }

  Range GetSecondaryRange(const gfx::Rect& bounds) const {
    switch (edge_) {
      case MAGNETISM_EDGE_TOP:
      case MAGNETISM_EDGE_BOTTOM:
        return Range(bounds.x(), bounds.right());
      case MAGNETISM_EDGE_LEFT:
      case MAGNETISM_EDGE_RIGHT:
        return Range(bounds.y(), bounds.bottom());
    }
    NOTREACHED();
    return Range();
  }

  static bool RangesIntersect(const Range& r1, const Range& r2) {
    return r2.first < r1.second && r2.second > r1.first;
  }

  
  const gfx::Rect bounds_;

  
  const MagnetismEdge edge_;

  
  
  
  Ranges ranges_;

  DISALLOW_COPY_AND_ASSIGN(MagnetismEdgeMatcher);
};

enum SecondaryMagnetismEdge {
  SECONDARY_MAGNETISM_EDGE_LEADING,
  SECONDARY_MAGNETISM_EDGE_TRAILING,
  SECONDARY_MAGNETISM_EDGE_NONE,
};

struct MatchedEdge {
  MagnetismEdge primary_edge;
  SecondaryMagnetismEdge secondary_edge;
};

class ASH_EXPORT MagnetismMatcher {
 public:
  static const int kMagneticDistance;

  
  MagnetismMatcher(const gfx::Rect& bounds, uint32 edges);
  ~MagnetismMatcher();

  
  
  
  bool ShouldAttach(const gfx::Rect& bounds, MatchedEdge* edge);

  
  bool AreEdgesObscured() const;

 private:
  
  void AttachToSecondaryEdge(const gfx::Rect& bounds,
                             MagnetismEdge edge,
                             SecondaryMagnetismEdge* secondary_edge) const;

  
  const int32 edges_;

  ScopedVector<MagnetismEdgeMatcher> matchers_;

  DISALLOW_COPY_AND_ASSIGN(MagnetismMatcher);
};

}  
}  

#endif  
