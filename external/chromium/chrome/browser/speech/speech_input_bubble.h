// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPEECH_SPEECH_INPUT_BUBBLE_H_
#define CHROME_BROWSER_SPEECH_SPEECH_INPUT_BUBBLE_H_
#pragma once

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/task.h"

namespace gfx {
class Canvas;
class Rect;
}
class SkBitmap;
class SkCanvas;
class TabContents;

class SpeechInputBubble {
 public:
  
  enum Button {
    BUTTON_TRY_AGAIN,
    BUTTON_CANCEL
  };

  
  class Delegate {
   public:
    
    
    virtual void InfoBubbleButtonClicked(Button button) = 0;

    
    
    
    
    virtual void InfoBubbleFocusChanged() = 0;

   protected:
    virtual ~Delegate() {
    }
  };

  
  
  
  
  
  static SpeechInputBubble* Create(TabContents* tab_contents,
                                   Delegate* delegate,
                                   const gfx::Rect& element_rect);

  
  
  static SpeechInputBubble* CreateNativeBubble(TabContents* tab_contents,
                                               Delegate* delegate,
                                               const gfx::Rect& element_rect);

  
  
  typedef SpeechInputBubble* (*FactoryMethod)(TabContents*,
                                              Delegate*,
                                              const gfx::Rect&);
  
  
  
#if defined(UNIT_TEST)
  static void set_factory(FactoryMethod factory) { factory_ = factory; }
#endif

  virtual ~SpeechInputBubble() {}

  
  
  virtual void SetWarmUpMode() = 0;

  
  
  virtual void SetRecordingMode() = 0;

  
  
  virtual void SetRecognizingMode() = 0;

  
  
  virtual void SetMessage(const string16& text) = 0;

  
  virtual void Show() = 0;

  
  
  virtual void Hide() = 0;

  
  virtual void SetInputVolume(float volume, float noise_volume) = 0;

  
  virtual TabContents* tab_contents() = 0;

  
  
  static const int kBubbleTargetOffsetX;

 private:
  static FactoryMethod factory_;
};

class SpeechInputBubbleBase : public SpeechInputBubble {
 public:
  
  
  enum DisplayMode {
    DISPLAY_MODE_WARM_UP,
    DISPLAY_MODE_RECORDING,
    DISPLAY_MODE_RECOGNIZING,
    DISPLAY_MODE_MESSAGE
  };

  explicit SpeechInputBubbleBase(TabContents* tab_contents);
  virtual ~SpeechInputBubbleBase();

  
  virtual void SetWarmUpMode();
  virtual void SetRecordingMode();
  virtual void SetRecognizingMode();
  virtual void SetMessage(const string16& text);
  virtual void SetInputVolume(float volume, float noise_volume);
  virtual TabContents* tab_contents();

 protected:
  
  virtual void UpdateLayout() = 0;

  
  virtual void UpdateImage() = 0;

  DisplayMode display_mode() {
    return display_mode_;
  }

  string16 message_text() {
    return message_text_;
  }

  SkBitmap icon_image();

 private:
  void DoRecognizingAnimationStep();
  void DoWarmingUpAnimationStep();
  void SetImage(const SkBitmap& image);

  void DrawVolumeOverlay(SkCanvas* canvas,
                         const SkBitmap& bitmap,
                         float volume);

  
  ScopedRunnableMethodFactory<SpeechInputBubbleBase> task_factory_;
  int animation_step_;  
  std::vector<SkBitmap> animation_frames_;
  std::vector<SkBitmap> warming_up_frames_;

  DisplayMode display_mode_;
  string16 message_text_;  
  
  scoped_ptr<SkBitmap> mic_image_;
  
  scoped_ptr<SkBitmap> buffer_image_;
  
  TabContents* tab_contents_;
  
  scoped_ptr<SkBitmap> icon_image_;
};

typedef SpeechInputBubble::Delegate SpeechInputBubbleDelegate;

#endif  
