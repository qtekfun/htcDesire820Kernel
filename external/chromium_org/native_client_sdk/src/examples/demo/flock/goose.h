// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOSE_H_
#define GOOSE_H_

#include <vector>
#include "ppapi/cpp/rect.h"

#include "vector2.h"

class Goose {
 public:
  
  Goose();

  
  Goose(const Vector2& location, const Vector2& velocity);

  
  
  
  
  
  
  
  
  void SimulationTick(const std::vector<Goose>& geese,
                      const std::vector<Vector2>& attractors,
                      const pp::Rect& flock_box);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  Vector2 DesiredVector(const std::vector<Goose>& geese,
                        const std::vector<Vector2>& attractors);

  
  
  
  
  Vector2 TurnTowardsTarget(const Vector2& target);

  
  Vector2 location() const {
    return location_;
  }
  Vector2 velocity() const {
    return velocity_;
  }

 private:
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t AccumulateSeparation(double distance,
                               const Vector2& goose_direction,
                               Vector2* separation, 
                               int32_t separation_count);

  
  
  
  
  
  
  
  
  
  
  
  
  int32_t AccumulateAlignment(double distance,
                              const Goose& goose,
                              Vector2* alignment, 
                              int32_t align_count);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t AccumulateCohesion(double distance,
                             const Goose& goose,
                             Vector2* cohesion, 
                             int32_t cohesion_count);

  Vector2 location_;
  Vector2 velocity_;
};

#endif  
