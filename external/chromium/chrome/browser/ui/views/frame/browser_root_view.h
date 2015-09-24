// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_ROOT_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_ROOT_VIEW_H_
#pragma once

#include "views/widget/root_view.h"

class AbstractTabStripView;
class BrowserView;

namespace ui {
class OSExchangeData;
}

class BrowserRootView : public views::RootView {
 public:
  
  BrowserRootView(BrowserView* browser_view, views::Widget* widget);

  
  virtual bool GetDropFormats(
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool AreDropTypesRequired() OVERRIDE;
  virtual bool CanDrop(const ui::OSExchangeData& data) OVERRIDE;
  virtual void OnDragEntered(const views::DropTargetEvent& event) OVERRIDE;
  virtual int OnDragUpdated(const views::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const views::DropTargetEvent& event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 private:
  
  bool ShouldForwardToTabStrip(const views::DropTargetEvent& event);

  
  
  views::DropTargetEvent* MapEventToTabStrip(
      const views::DropTargetEvent& event,
      const ui::OSExchangeData& data);

  inline AbstractTabStripView* tabstrip() const;

  
  
  
  bool GetPasteAndGoURL(const ui::OSExchangeData& data, GURL* url);

  
  BrowserView* browser_view_;

  
  
  
  bool forwarding_to_tab_strip_;

  DISALLOW_COPY_AND_ASSIGN(BrowserRootView);
};

#endif  
