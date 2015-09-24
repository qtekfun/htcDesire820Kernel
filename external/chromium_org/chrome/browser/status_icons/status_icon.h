// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STATUS_ICONS_STATUS_ICON_H_
#define CHROME_BROWSER_STATUS_ICONS_STATUS_ICON_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "chrome/browser/status_icons/status_icon_menu_model.h"

namespace gfx {
class ImageSkia;
}

class StatusIconObserver;

class StatusIcon {
 public:
  StatusIcon();
  virtual ~StatusIcon();

  
  virtual void SetImage(const gfx::ImageSkia& image) = 0;

  
  virtual void SetPressedImage(const gfx::ImageSkia& image) = 0;

  
  
  
  
  virtual void SetToolTip(const base::string16& tool_tip) = 0;

  
  
  virtual void DisplayBalloon(const gfx::ImageSkia& icon,
                              const base::string16& title,
                              const base::string16& contents) = 0;

  
  
  void SetContextMenu(scoped_ptr<StatusIconMenuModel> menu);

  
  
  
  
  void AddObserver(StatusIconObserver* observer);
  void RemoveObserver(StatusIconObserver* observer);

  
  bool HasObservers() const;

  
  void DispatchClickEvent();
#if defined(OS_WIN)
  void DispatchBalloonClickEvent();
#endif

 protected:
  
  
  
  virtual void UpdatePlatformContextMenu(StatusIconMenuModel* model) = 0;

 private:
  ObserverList<StatusIconObserver> observers_;

  
  scoped_ptr<StatusIconMenuModel> context_menu_contents_;

  DISALLOW_COPY_AND_ASSIGN(StatusIcon);
};

#endif  
