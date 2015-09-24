// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_INFOBARS_INTERNAL_SUBCLASSING_WINDOW_WITH_DELEGATE_H_
#define CHROME_FRAME_INFOBARS_INTERNAL_SUBCLASSING_WINDOW_WITH_DELEGATE_H_

#include <atlbase.h>
#include <atlcrack.h>
#include <atlwin.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "chrome_frame/pin_module.h"

template<typename T> class SubclassingWindowWithDelegate
    : public CWindowImpl<T> {
 public:
  
  
  
  class Delegate {
   public:
    
    virtual ~Delegate() {}

    
    
    
    virtual void AdjustDisplacedWindowDimensions(RECT* rect) = 0;
  };  

  SubclassingWindowWithDelegate() {}

  
  
  
  
  
  
  
  bool Initialize(HWND hwnd, Delegate* delegate) {
    DCHECK(delegate != NULL);
    DCHECK(delegate_ == NULL);
    scoped_ptr<Delegate> new_delegate(delegate);

    if (!::IsWindow(hwnd) || !SubclassWindow(hwnd)) {
      PLOG(ERROR) << "Failed to subclass an HWND";
      return false;
    }

    
    
    chrome_frame::PinModule();

    delegate_.swap(new_delegate);

    return true;
  }

  
  static Delegate* GetDelegateForHwnd(HWND hwnd) {
    return reinterpret_cast<Delegate*>(
        ::SendMessage(hwnd, RegisterGetDelegateMessage(), NULL, NULL));
  }

  BEGIN_MSG_MAP_EX(SubclassingWindowWithDelegate)
    MESSAGE_HANDLER(RegisterGetDelegateMessage(), OnGetDelegate)
    MSG_WM_DESTROY(OnDestroy)
  END_MSG_MAP()

  
  virtual void OnFinalMessage(HWND hwnd) {
    delete this;
  }

 protected:
  scoped_ptr<Delegate>& delegate() { return delegate_; }

 private:
  
  static UINT RegisterGetDelegateMessage() {
    static UINT message_id(
        RegisterWindowMessage(L"SubclassingWindowWithDelegate::OnGetDelegate"));
    return message_id;
  }

  
  
  void OnDestroy() {
    delegate_.reset();
  }

  LRESULT OnGetDelegate(UINT message,
                        WPARAM wparam,
                        LPARAM lparam,
                        BOOL& handled) {
    return reinterpret_cast<LRESULT>(delegate_.get());
  }

  scoped_ptr<Delegate> delegate_;
  DISALLOW_COPY_AND_ASSIGN(SubclassingWindowWithDelegate);
};  

#endif  
