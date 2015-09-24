// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_WIN_SINGLETON_HWND_H_
#define UI_GFX_WIN_SINGLETON_HWND_H_

#include <windows.h>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/observer_list.h"
#include "ui/gfx/win/window_impl.h"

template<typename T> struct DefaultSingletonTraits;

namespace gfx {

class GFX_EXPORT SingletonHwnd : public WindowImpl {
 public:
  static SingletonHwnd* GetInstance();

  
  class Observer {
   public:
    virtual void OnWndProc(HWND hwnd,
                           UINT message,
                           WPARAM wparam,
                           LPARAM lparam) = 0;
  };

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  virtual BOOL ProcessWindowMessage(HWND window,
                                    UINT message,
                                    WPARAM wparam,
                                    LPARAM lparam,
                                    LRESULT& result,
                                    DWORD msg_map_id) OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<SingletonHwnd>;

  SingletonHwnd();
  ~SingletonHwnd();

  
  ObserverList<Observer> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(SingletonHwnd);
};

}  

#endif  
