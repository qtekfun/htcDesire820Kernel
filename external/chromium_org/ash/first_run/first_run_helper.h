// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_FIRST_RUN_FIRST_RUN_HELPER_H_
#define ASH_FIRST_RUN_FIRST_RUN_HELPER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/observer_list.h"

namespace gfx {
class Rect;
}

namespace views {
class Widget;
}

namespace ash {

class ASH_EXPORT FirstRunHelper {
 public:
  class Observer {
   public:
    
    virtual void OnCancelled() = 0;
    virtual ~Observer() {}
  };

 public:
  FirstRunHelper();
  virtual ~FirstRunHelper();

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  virtual views::Widget* GetOverlayWidget() = 0;

  
  virtual void OpenAppList() = 0;
  virtual void CloseAppList() = 0;

  
  virtual gfx::Rect GetLauncherBounds() = 0;

  
  virtual gfx::Rect GetAppListButtonBounds() = 0;

  
  
  virtual gfx::Rect GetAppListBounds() = 0;

  
  virtual void OpenTrayBubble() = 0;
  virtual void CloseTrayBubble() = 0;

  
  virtual bool IsTrayBubbleOpened() = 0;

  
  
  virtual gfx::Rect GetTrayBubbleBounds() = 0;

  
  
  virtual gfx::Rect GetHelpButtonBounds() = 0;

 protected:
  ObserverList<Observer>& observers() { return observers_; }

 private:
  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(FirstRunHelper);
};

}  

#endif  

