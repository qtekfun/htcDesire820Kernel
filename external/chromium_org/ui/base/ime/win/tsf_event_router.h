// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_WIN_TSF_EVENT_ROUTER_H_
#define UI_BASE_IME_WIN_TSF_EVENT_ROUTER_H_

#include <atlbase.h>
#include <atlcom.h>
#include <msctf.h>

#include <set>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/range/range.h"

struct ITfDocumentMgr;

namespace ui {

class TSFEventRouterObserver {
 public:
  TSFEventRouterObserver() {}

  
  virtual void OnCandidateWindowCountChanged(size_t window_count) {}

  
  virtual void OnTSFStartComposition() {}

  
  
  virtual void OnTextUpdated(const gfx::Range& composition_range) {}

  
  virtual void OnTSFEndComposition() {}

 protected:
  virtual ~TSFEventRouterObserver() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(TSFEventRouterObserver);
};

class UI_EXPORT TSFEventRouter {
 public:
  
  explicit TSFEventRouter(TSFEventRouterObserver* observer);
  virtual ~TSFEventRouter();

  
  bool IsImeComposing();

  
  void OnCandidateWindowCountChanged(size_t window_count);
  void OnTSFStartComposition();
  void OnTextUpdated(const gfx::Range& composition_range);
  void OnTSFEndComposition();

  
  void SetManager(ITfThreadMgr* thread_manager);

 private:
  class Delegate;

  CComPtr<Delegate> delegate_;

  TSFEventRouterObserver* observer_;

  DISALLOW_COPY_AND_ASSIGN(TSFEventRouter);
};

}  

#endif  
