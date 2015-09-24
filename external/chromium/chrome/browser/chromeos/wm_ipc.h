// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_WM_IPC_H_
#define CHROME_BROWSER_CHROMEOS_WM_IPC_H_
#pragma once

#include <gtk/gtk.h>
#include <map>
#include <string>
#include <vector>

#include "base/logging.h"
#include "third_party/cros/chromeos_wm_ipc_enums.h"

typedef unsigned long Atom;
typedef unsigned long XID;

namespace base {
template <typename T> struct DefaultLazyInstanceTraits;
}

namespace chromeos {

class WmIpc {
 public:
  enum AtomType {
    ATOM_CHROME_LOGGED_IN = 0,
    ATOM_CHROME_WINDOW_TYPE,
    ATOM_CHROME_WM_MESSAGE,
    ATOM_MANAGER,
    ATOM_STRING,
    ATOM_UTF8_STRING,
    ATOM_WM_S0,
    kNumAtoms,
  };

  struct Message {
   public:
    Message() {
      Init(WM_IPC_MESSAGE_UNKNOWN);
    }
    
    explicit Message(WmIpcMessageType type) {
      Init(type);
    }

    WmIpcMessageType type() const { return type_; }
    void set_type(WmIpcMessageType type) { type_ = type; }

    inline int max_params() const {
      return arraysize(params_);
    }
    long param(int index) const {
      DCHECK_GE(index, 0);
      DCHECK_LT(index, max_params());
      return params_[index];
    }
    void set_param(int index, long value) {
      DCHECK_GE(index, 0);
      DCHECK_LT(index, max_params());
      params_[index] = value;
    }

   private:
    
    void Init(WmIpcMessageType type) {
      set_type(type);
      for (int i = 0; i < max_params(); ++i) {
        set_param(i, 0);
      }
    }

    
    WmIpcMessageType type_;

    
    
    
    long params_[4];
  };

  
  static WmIpc* instance();

  
  
  
  
  bool SetWindowType(GtkWidget* widget,
                     WmIpcWindowType type,
                     const std::vector<int>* params);

  
  
  
  
  WmIpcWindowType GetWindowType(GtkWidget* widget, std::vector<int>* params);

  
  void SendMessage(const Message& msg);

  
  
  bool DecodeMessage(const GdkEventClient& event, Message* msg);

  
  
  
  
  void HandleNonChromeClientMessageEvent(const GdkEventClient& event);

  
  
  void SetLoggedInProperty(bool logged_in);

  
  void NotifyAboutSignout();

 private:
  friend struct base::DefaultLazyInstanceTraits<WmIpc>;

  WmIpc();

  
  
  
  void InitWmInfo();

  
  
  
  
  std::map<AtomType, Atom> type_to_atom_;
  std::map<Atom, std::string> atom_to_string_;

  
  Atom wm_message_atom_;

  
  XID wm_;

  DISALLOW_COPY_AND_ASSIGN(WmIpc);
};

}  

#endif  
