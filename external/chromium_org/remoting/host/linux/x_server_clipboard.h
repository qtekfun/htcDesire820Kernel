// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_HOST_LINUX_X_SERVER_CLIPBOARD_H_
#define REMOTING_HOST_LINUX_X_SERVER_CLIPBOARD_H_

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/timer/timer.h"

namespace remoting {

class XServerClipboard {
 public:
  
  
  
  
  
  
  typedef base::Callback<void(const std::string& mime_type,
                              const std::string& data)>
      ClipboardChangedCallback;

  XServerClipboard();
  ~XServerClipboard();

  
  
  
  void Init(Display* display, const ClipboardChangedCallback& callback);

  
  
  void SetClipboard(const std::string& mime_type, const std::string& data);

  
  
  void ProcessXEvent(XEvent* event);

 private:
  
  void OnSetSelectionOwnerNotify(Atom selection, Time timestamp);
  void OnPropertyNotify(XEvent* event);
  void OnSelectionNotify(XEvent* event);
  void OnSelectionRequest(XEvent* event);
  void OnSelectionClear(XEvent* event);

  
  
  
  
  void SendTargetsResponse(Window requestor, Atom property);
  void SendTimestampResponse(Window requestor, Atom property);
  void SendStringResponse(Window requestor, Atom property, Atom target);

  
  
  
  
  
  void HandleSelectionNotify(XSelectionEvent* event,
                             Atom type,
                             int format,
                             int item_count,
                             void* data);

  
  
  
  bool HandleSelectionTargetsEvent(XSelectionEvent* event,
                                   int format,
                                   int item_count,
                                   void* data);
  bool HandleSelectionStringEvent(XSelectionEvent* event,
                                  int format,
                                  int item_count,
                                  void* data);

  
  void NotifyClipboardText(const std::string& text);

  
  
  void RequestSelectionTargets(Atom selection);
  void RequestSelectionString(Atom selection, Atom target);

  
  void AssertSelectionOwnership(Atom selection);
  bool IsSelectionOwner(Atom selection);

  
  Display* display_;

  
  
  Window clipboard_window_;

  
  
  
  int xfixes_event_base_;

  
  Atom clipboard_atom_;
  Atom large_selection_atom_;
  Atom selection_string_atom_;
  Atom targets_atom_;
  Atom timestamp_atom_;
  Atom utf8_string_atom_;

  
  
  std::set<Atom> selections_owned_;

  
  
  std::string data_;

  
  
  Atom large_selection_property_;

  
  
  
  base::TimeTicks get_selections_time_;

  
  ClipboardChangedCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(XServerClipboard);
};

}  

#endif  
