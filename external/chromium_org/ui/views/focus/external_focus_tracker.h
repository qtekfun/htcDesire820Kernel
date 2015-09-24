// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_FOCUS_EXTERNAL_FOCUS_TRACKER_H_
#define UI_VIEWS_FOCUS_EXTERNAL_FOCUS_TRACKER_H_

#include "base/compiler_specific.h"
#include "ui/views/focus/focus_manager.h"

namespace views {

class View;
class ViewStorage;

class VIEWS_EXPORT ExternalFocusTracker : public FocusChangeListener {
 public:
  ExternalFocusTracker(View* parent_view, FocusManager* focus_manager);
  virtual ~ExternalFocusTracker();

  
  virtual void OnWillChangeFocus(View* focused_before,
                                 View* focused_now) OVERRIDE;
  virtual void OnDidChangeFocus(View* focused_before,
                                View* focused_now) OVERRIDE;

  
  
  
  void FocusLastFocusedExternalView();

  
  
  
  
  
  void SetFocusManager(FocusManager* focus_manager);

 private:
  
  
  void StoreLastFocusedView(View* view);

  
  
  void StartTracking();

  
  FocusManager* focus_manager_;

  
  int last_focused_view_storage_id_;

  
  
  ViewStorage* view_storage_;

  
  
  View* parent_view_;

  DISALLOW_COPY_AND_ASSIGN(ExternalFocusTracker);
};

}  

#endif  
