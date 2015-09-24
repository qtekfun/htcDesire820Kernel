// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_TAP_SUPPRESSION_CONTROLLER_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_TAP_SUPPRESSION_CONTROLLER_H_

#include "base/time/time.h"
#include "base/timer/timer.h"
#include "content/common/content_export.h"

namespace content {

class TapSuppressionControllerClient;

class CONTENT_EXPORT TapSuppressionController {
 public:
  explicit TapSuppressionController(TapSuppressionControllerClient* client);
  virtual ~TapSuppressionController();

  
  void GestureFlingCancel();

  
  
  
  void GestureFlingCancelAck(bool processed);

  
  
  
  bool ShouldDeferTapDown();

  
  
  bool ShouldSuppressTapEnd();

 protected:
  virtual base::TimeTicks Now();
  virtual void StartTapDownTimer(const base::TimeDelta& delay);
  virtual void StopTapDownTimer();
  void TapDownTimerExpired();

 private:
  friend class MockTapSuppressionController;

  enum State {
    NOTHING,
    GFC_IN_PROGRESS,
    TAP_DOWN_STASHED,
    LAST_CANCEL_STOPPED_FLING,
  };


  TapSuppressionControllerClient* client_;
  base::OneShotTimer<TapSuppressionController> tap_down_timer_;
  State state_;

  
  
  
  base::TimeTicks fling_cancel_time_;

  DISALLOW_COPY_AND_ASSIGN(TapSuppressionController);
};

}  

#endif  
