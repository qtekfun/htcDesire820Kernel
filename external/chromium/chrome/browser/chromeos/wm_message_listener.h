// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_WM_MESSAGE_LISTENER_H_
#define CHROME_BROWSER_CHROMEOS_WM_MESSAGE_LISTENER_H_
#pragma once

#include <gtk/gtk.h>

#include "base/memory/singleton.h"
#include "base/message_loop.h"
#include "base/observer_list.h"
#include "chrome/browser/chromeos/wm_ipc.h"

class Browser;
class BrowserView;

namespace chromeos {

class WmMessageListener : public MessageLoopForUI::Observer {
 public:
  
  
  class Observer {
   public:
    virtual void ProcessWmMessage(const WmIpc::Message& message,
                                  GdkWindow* window) = 0;
  };

  static WmMessageListener* GetInstance();

  void AddObserver(Observer* observer) {
    observers_.AddObserver(observer);
  }

  void RemoveObserver(Observer* observer) {
    observers_.RemoveObserver(observer);
  }

  
  virtual void WillProcessEvent(GdkEvent* event);
  virtual void DidProcessEvent(GdkEvent* event);

 private:
  friend struct DefaultSingletonTraits<WmMessageListener>;

  WmMessageListener();
  ~WmMessageListener();

  
  void ProcessMessage(const WmIpc::Message& message, GdkWindow* window);

  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(WmMessageListener);
};

}  

#endif  
