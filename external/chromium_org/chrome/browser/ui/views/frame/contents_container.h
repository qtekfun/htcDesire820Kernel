// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_CONTENTS_CONTAINER_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_CONTENTS_CONTAINER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/view.h"

class ContentsContainer : public views::View {
 public:
  explicit ContentsContainer(views::View* active_web_view);
  virtual ~ContentsContainer();

  
  
  
  
  bool SetActiveTopMargin(int margin);

  
  virtual void Layout() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

 private:
  views::View* active_web_view_;

  
  
  int active_top_margin_;

  DISALLOW_COPY_AND_ASSIGN(ContentsContainer);
};

#endif  
