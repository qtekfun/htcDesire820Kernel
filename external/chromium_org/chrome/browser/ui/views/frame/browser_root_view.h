// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_ROOT_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_ROOT_VIEW_H_

#include "ui/views/widget/root_view.h"

class BrowserView;
class TabStrip;

namespace ui {
class OSExchangeData;
}

class BrowserRootView : public views::internal::RootView {
 public:
  
  static const char kViewClassName[];

  
  BrowserRootView(BrowserView* browser_view, views::Widget* widget);

  
  virtual bool GetDropFormats(
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool AreDropTypesRequired() OVERRIDE;
  virtual bool CanDrop(const ui::OSExchangeData& data) OVERRIDE;
  virtual void OnDragEntered(const ui::DropTargetEvent& event) OVERRIDE;
  virtual int OnDragUpdated(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;

  
  virtual void DispatchGestureEvent(ui::GestureEvent* event) OVERRIDE;

 private:
  
  bool ShouldForwardToTabStrip(const ui::DropTargetEvent& event);

  
  
  ui::DropTargetEvent* MapEventToTabStrip(
      const ui::DropTargetEvent& event,
      const ui::OSExchangeData& data);

  inline TabStrip* tabstrip() const;

  
  
  
  bool GetPasteAndGoURL(const ui::OSExchangeData& data, GURL* url);

  
  BrowserView* browser_view_;

  
  
  
  bool forwarding_to_tab_strip_;

  DISALLOW_COPY_AND_ASSIGN(BrowserRootView);
};

#endif  
