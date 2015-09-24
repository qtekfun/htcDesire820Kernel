// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_IMAGE_IMAGE_FAMILY_H_
#define UI_GFX_IMAGE_IMAGE_FAMILY_H_

#include <iterator>
#include <map>
#include <utility>

#include "ui/gfx/gfx_export.h"
#include "ui/gfx/image/image.h"

namespace gfx {
class ImageSkia;
class Size;

class GFX_EXPORT ImageFamily {
 private:
  
  
  struct MapKey : std::pair<float, int> {
    MapKey(float aspect, int width)
        : std::pair<float, int>(aspect, width) {}

    float aspect() const { return first; }

    int width() const { return second; }
  };

 public:
  
  
  class GFX_EXPORT const_iterator :
    std::iterator<std::bidirectional_iterator_tag, const gfx::Image> {
   public:
    const_iterator();

    const_iterator(const const_iterator& other);

    ~const_iterator();

    const_iterator& operator++() {
      ++map_iterator_;
      return *this;
    }

    const_iterator operator++(int ) {
      const_iterator result(*this);
      ++(*this);
      return result;
    }

    const_iterator& operator--() {
      --map_iterator_;
      return *this;
    }

    const_iterator operator--(int ) {
      const_iterator result(*this);
      --(*this);
      return result;
    }

    bool operator==(const const_iterator& other) const {
      return map_iterator_ == other.map_iterator_;
    }

    bool operator!=(const const_iterator& other) const {
      return map_iterator_ != other.map_iterator_;
    }

    const gfx::Image& operator*() const {
      return map_iterator_->second;
    }

    const gfx::Image* operator->() const {
      return &**this;
    }

   private:
    friend class ImageFamily;

    explicit const_iterator(
        const std::map<MapKey, gfx::Image>::const_iterator& other);

    std::map<MapKey, gfx::Image>::const_iterator map_iterator_;
  };

  ImageFamily();
  ~ImageFamily();

  
  const_iterator begin() const { return const_iterator(map_.begin()); }
  
  const_iterator end() const { return const_iterator(map_.end()); }

  
  bool empty() const { return map_.empty(); }

  
  void clear() { return map_.clear(); }

  
  // same size, it will be overwritten.
  void Add(const gfx::Image& image);

  
  // same size, it will be overwritten.
  void Add(const gfx::ImageSkia& image_skia);

  
  
  
  
  
  
  const gfx::Image* GetBest(int width, int height) const;

  
  
  
  
  
  
  
  const gfx::Image* GetBest(const gfx::Size& size) const;

 private:
  
  
  
  float GetClosestAspect(float desired_aspect) const;

  
  
  
  
  
  const gfx::Image* GetWithExactAspect(float aspect, int width) const;

  
  std::map<MapKey, gfx::Image> map_;
};

}  

#endif  
