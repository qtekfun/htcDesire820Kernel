// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_V2_PUBLIC_VIEW_H_
#define UI_V2_PUBLIC_VIEW_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/compositor/layer_type.h"
#include "ui/gfx/rect.h"
#include "ui/v2/public/layout.h"
#include "ui/v2/public/painter.h"
#include "ui/v2/public/v2_export.h"

namespace ui {
class Layer;
}

namespace v2 {

class Layout;
class Painter;
class ViewObserver;
class ViewLayerOwner;

class V2_EXPORT View  {
 public:
  typedef std::vector<View*> Children;

  View();
  virtual ~View();

  

  void set_owned_by_parent(bool owned_by_parent) {
    owned_by_parent_ = owned_by_parent;
  }

  
  void SetPainter(Painter* painter);

  
  
  
  void SetLayout(Layout* layout);

  

  void AddObserver(ViewObserver* observer);
  void RemoveObserver(ViewObserver* observer);

  

  void SetBounds(const gfx::Rect& bounds);
  gfx::Rect bounds() const { return bounds_; }

  void SetVisible(bool visible);
  bool visible() const { return visible_; }

  

  View* parent() { return parent_; }
  const View* parent() const { return parent_; }
  const Children& children() const { return children_; }

  void AddChild(View* child);
  void RemoveChild(View* child);

  bool Contains(View* child) const;

  void StackChildAtTop(View* child);
  void StackChildAtBottom(View* child);
  void StackChildAbove(View* child, View* other);
  void StackChildBelow(View* child, View* other);

  

  inline const ui::Layer* layer() const;
  inline ui::Layer* layer();
  bool HasLayer() const;
  void CreateLayer(ui::LayerType layer_type);
  void DestroyLayer();
  ui::Layer* AcquireLayer();

 private:
  friend class ViewPrivate;

  
  gfx::Rect bounds_;
  bool visible_;

  scoped_ptr<Painter> painter_;
  scoped_ptr<Layout> layout_;

  
  bool owned_by_parent_;
  View* parent_;
  Children children_;

  
  scoped_ptr<ViewLayerOwner> layer_owner_;

  ObserverList<ViewObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(View);
};

}  

#endif  
