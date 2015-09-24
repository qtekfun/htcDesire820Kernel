// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_NATIVE_PANEL_STACK_WINDOW_H_
#define CHROME_BROWSER_UI_PANELS_NATIVE_PANEL_STACK_WINDOW_H_

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "ui/gfx/image/image.h"

class Panel;
namespace gfx {
class Rect;
class Vector2d;
}

class NativePanelStackWindowDelegate {
 public:
  
  virtual base::string16 GetTitle() const = 0;

  
  virtual gfx::Image GetIcon() const = 0;

  
  virtual void PanelBoundsBatchUpdateCompleted() = 0;
};

class NativePanelStackWindow {
 public:
  
  
  static NativePanelStackWindow* Create(
      NativePanelStackWindowDelegate* delegate);

  virtual ~NativePanelStackWindow() {}

  virtual bool IsMinimized() const = 0;

 protected:
  friend class StackedPanelCollection;

  
  
  virtual void Close() = 0;

  

  
  
  virtual void AddPanel(Panel* panel) = 0;

  
  
  virtual void RemovePanel(Panel* panel) = 0;

  
  
  virtual void MergeWith(NativePanelStackWindow* another) = 0;

  
  virtual bool IsEmpty() const = 0;

  
  virtual bool HasPanel(Panel* panel) const = 0;

  
  
  virtual void MovePanelsBy(const gfx::Vector2d& delta) = 0;

  
  virtual void BeginBatchUpdatePanelBounds(bool animate) = 0;
  virtual void AddPanelBoundsForBatchUpdate(Panel* panel,
                                            const gfx::Rect& new_bounds) = 0;
  virtual void EndBatchUpdatePanelBounds() = 0;

  
  
  virtual bool IsAnimatingPanelBounds() const = 0;

  
  virtual void Minimize() = 0;

  
  
  virtual void DrawSystemAttention(bool draw_attention) = 0;

  
  virtual void OnPanelActivated(Panel* panel) = 0;
};

#endif  
