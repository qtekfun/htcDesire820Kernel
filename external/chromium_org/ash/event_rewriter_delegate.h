// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_EVENT_REWRITER_DELEGATE_H_
#define ASH_EVENT_REWRITER_DELEGATE_H_

namespace ui {
class KeyEvent;
class LocatedEvent;
}  

namespace ash {

class EventRewriterDelegate {
 public:
  enum Action {
    ACTION_REWRITE_EVENT,
    ACTION_DROP_EVENT,
  };

  virtual ~EventRewriterDelegate() {}

  
  
  
  virtual Action RewriteOrFilterKeyEvent(ui::KeyEvent* event) = 0;
  virtual Action RewriteOrFilterLocatedEvent(ui::LocatedEvent* event) = 0;
};

}  

#endif  
