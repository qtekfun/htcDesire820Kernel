// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_APP_LIST_DRAG_AND_DROP_HOST_H_
#define UI_APP_LIST_VIEWS_APP_LIST_DRAG_AND_DROP_HOST_H_

#include <string>

namespace gfx {
class Point;
class Vector2d;
}  

namespace app_list {

class ApplicationDragAndDropHost {
 public:
  
  
  
  
  
  
  virtual void CreateDragIconProxy(
      const gfx::Point& location_in_screen_coordinates,
      const gfx::ImageSkia& icon,
      views::View* replaced_view,
      const gfx::Vector2d& cursor_offset_from_center,
      float scale_factor) = 0;

  
  virtual void UpdateDragIconProxy(
      const gfx::Point& location_in_screen_coordinates) = 0;

  
  virtual void DestroyDragIconProxy() = 0;

  
  
  
  
  virtual bool StartDrag(const std::string& app_id,
                         const gfx::Point& location_in_screen_coordinates) = 0;

  
  
  
  
  
  virtual bool Drag(const gfx::Point& location_in_screen_coordinates) = 0;

  
  
  
  virtual void EndDrag(bool cancel) = 0;
};

}  

#endif  
