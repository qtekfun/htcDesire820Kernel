// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_WEBPLUGIN_IME_WIN_H_
#define CONTENT_CHILD_NPAPI_WEBPLUGIN_IME_WIN_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "third_party/npapi/bindings/npapi.h"
#include "ui/gfx/rect.h"

namespace content {

class PluginInstance;

class WebPluginIMEWin {
 public:
  
  
  class ScopedLock {
   public:
    explicit ScopedLock(WebPluginIMEWin* instance) : instance_(instance) {
      if (instance_)
        instance_->Lock();
    }
    ~ScopedLock() {
      if (instance_)
        instance_->Unlock();
    }

   private:
    WebPluginIMEWin* instance_;
  };

  WebPluginIMEWin();
  ~WebPluginIMEWin();

  
  
  
  
  
  void CompositionUpdated(const base::string16& text,
                          std::vector<int> clauses,
                          std::vector<int> target,
                          int cursor_position);
  void CompositionCompleted(const base::string16& text);

  
  bool SendEvents(PluginInstance* instance);

  
  bool GetStatus(int* input_type, gfx::Rect* caret_rect);

  
  
  
  static FARPROC GetProcAddress(const char* name);

 private:
  
  
  
  
  
  void Lock();
  void Unlock();

  
  static WebPluginIMEWin* GetInstance(HIMC context);

  
  static BOOL WINAPI ImmAssociateContextEx(HWND window,
                                           HIMC context,
                                           DWORD flags);
  static LONG WINAPI ImmGetCompositionStringW(HIMC context,
                                              DWORD index,
                                              LPVOID dst_data,
                                              DWORD dst_size);
  static HIMC WINAPI ImmGetContext(HWND window);
  static BOOL WINAPI ImmReleaseContext(HWND window, HIMC context);
  static BOOL WINAPI ImmSetCandidateWindow(HIMC context,
                                           CANDIDATEFORM* candidate);
  static BOOL WINAPI ImmSetOpenStatus(HIMC context, BOOL open);

  
  std::vector<NPEvent> events_;

  
  base::string16 composition_text_;

  
  base::string16 result_text_;

  
  std::string composition_attributes_;

  
  std::vector<uint32> composition_clauses_;

  
  uint32 result_clauses_[2];

  
  int cursor_position_;

  
  int delta_start_;

  
  
  bool composing_text_;

  
  
  
  bool support_ime_messages_;

  
  bool status_updated_;
  int input_type_;
  gfx::Rect caret_rect_;

  
  static WebPluginIMEWin* instance_;
};

}  

#endif  
