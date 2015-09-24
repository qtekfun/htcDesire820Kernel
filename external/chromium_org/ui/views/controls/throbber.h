// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_THROBBER_H_
#define UI_VIEWS_CONTROLS_THROBBER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "ui/views/view.h"

namespace gfx {
class ImageSkia;
}

namespace views {


class VIEWS_EXPORT Throbber : public View {
 public:
  
  
  
  
  Throbber(int frame_time_ms, bool paint_while_stopped);
  Throbber(int frame_time_ms, bool paint_while_stopped, gfx::ImageSkia* frames);
  virtual ~Throbber();

  
  virtual void Start();
  virtual void Stop();

  
  void SetFrames(const gfx::ImageSkia* frames);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 protected:
  
  bool running_;

 private:
  void Run();

  bool paint_while_stopped_;
  int frame_count_;  
  base::Time start_time_;  
  const gfx::ImageSkia* frames_;  
  base::TimeDelta frame_time_;  
  base::RepeatingTimer<Throbber> timer_;  

  DISALLOW_COPY_AND_ASSIGN(Throbber);
};

class VIEWS_EXPORT SmoothedThrobber : public Throbber {
 public:
  explicit SmoothedThrobber(int frame_delay_ms);
  SmoothedThrobber(int frame_delay_ms, gfx::ImageSkia* frames);
  virtual ~SmoothedThrobber();

  virtual void Start() OVERRIDE;
  virtual void Stop() OVERRIDE;

  void set_start_delay_ms(int value) { start_delay_ms_ = value; }
  void set_stop_delay_ms(int value) { stop_delay_ms_ = value; }

 private:
  
  
  void StartDelayOver();

  
  
  void StopDelayOver();

  
  int start_delay_ms_;

  
  int stop_delay_ms_;

  base::OneShotTimer<SmoothedThrobber> start_timer_;
  base::OneShotTimer<SmoothedThrobber> stop_timer_;

  DISALLOW_COPY_AND_ASSIGN(SmoothedThrobber);
};

class VIEWS_EXPORT CheckmarkThrobber : public Throbber {
 public:
  CheckmarkThrobber();

  
  
  void SetChecked(bool checked);

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 private:
  static const int kFrameTimeMs = 30;

  
  bool checked_;

  
  const gfx::ImageSkia* checkmark_;

  DISALLOW_COPY_AND_ASSIGN(CheckmarkThrobber);
};

}  

#endif  
