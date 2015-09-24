// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STATUS_ICONS_STATUS_ICON_H_
#define CHROME_BROWSER_STATUS_ICONS_STATUS_ICON_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/string16.h"

class SkBitmap;

namespace ui {
class MenuModel;
}

class StatusIcon {
 public:
  StatusIcon();
  virtual ~StatusIcon();

  
  virtual void SetImage(const SkBitmap& image) = 0;

  
  virtual void SetPressedImage(const SkBitmap& image) = 0;

  
  virtual void SetToolTip(const string16& tool_tip) = 0;

  
  virtual void DisplayBalloon(const string16& title,
                              const string16& contents) = 0;

  
  
  void SetContextMenu(ui::MenuModel* menu);

  class Observer {
   public:
    virtual ~Observer() {}

    
    
    
    
    
    virtual void OnClicked() = 0;
  };

  
  
  
  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  bool HasObservers();

  
  void DispatchClickEvent();

 protected:
  
  
  
  virtual void UpdatePlatformContextMenu(ui::MenuModel* model) = 0;

 private:
  ObserverList<Observer> observers_;
  
  scoped_ptr<ui::MenuModel> context_menu_contents_;
  DISALLOW_COPY_AND_ASSIGN(StatusIcon);
};

#endif  
