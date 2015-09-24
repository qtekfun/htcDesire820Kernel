// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPEECH_SPEECH_RECOGNITION_BUBBLE_H_
#define CHROME_BROWSER_SPEECH_SPEECH_RECOGNITION_BUBBLE_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "ui/base/layout.h"
#include "ui/gfx/image/image_skia.h"

class SkBitmap;
class SkCanvas;

namespace content {
class WebContents;
}

namespace gfx {
class Canvas;
class Rect;
}

class SpeechRecognitionBubble {
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

  
  
  
  
  
  
  static SpeechRecognitionBubble* Create(
      int render_process_id,
      int render_view_id,
      Delegate* delegate,
      const gfx::Rect& element_rect);

  
  
  static SpeechRecognitionBubble* CreateNativeBubble(
      int render_process_id,
      int render_view_id,
      Delegate* delegate,
      const gfx::Rect& element_rect);

  
  
  typedef SpeechRecognitionBubble* (*FactoryMethod)(content::WebContents*,
                                                    Delegate*,
                                                    const gfx::Rect&);
  
  
  
#if defined(UNIT_TEST)
  static void set_factory(FactoryMethod factory) { factory_ = factory; }
#endif

  virtual ~SpeechRecognitionBubble() {}

  
  
  virtual void SetWarmUpMode() = 0;

  
  
  virtual void SetRecordingMode() = 0;

  
  
  virtual void SetRecognizingMode() = 0;

  
  
  virtual void SetMessage(const base::string16& text) = 0;

  
  virtual void Show() = 0;

  
  
  virtual void Hide() = 0;

  
  virtual void SetInputVolume(float volume, float noise_volume) = 0;

  
  virtual content::WebContents* GetWebContents() = 0;

  
  
  static const int kBubbleTargetOffsetX;

 private:
  static FactoryMethod factory_;
};

class SpeechRecognitionBubbleBase : public SpeechRecognitionBubble {
 public:
  
  
  enum DisplayMode {
    DISPLAY_MODE_WARM_UP,
    DISPLAY_MODE_RECORDING,
    DISPLAY_MODE_RECOGNIZING,
    DISPLAY_MODE_MESSAGE
  };

  SpeechRecognitionBubbleBase(int render_process_id, int render_view_id);
  virtual ~SpeechRecognitionBubbleBase();

  
  virtual void SetWarmUpMode() OVERRIDE;
  virtual void SetRecordingMode() OVERRIDE;
  virtual void SetRecognizingMode() OVERRIDE;
  virtual void SetMessage(const base::string16& text) OVERRIDE;
  virtual void SetInputVolume(float volume, float noise_volume) OVERRIDE;
  virtual content::WebContents* GetWebContents() OVERRIDE;

 protected:
  
  virtual void UpdateLayout() = 0;

  
  virtual void UpdateImage() = 0;

  DisplayMode display_mode() const { return display_mode_; }

  const base::string16& message_text() const { return message_text_; }

  gfx::ImageSkia icon_image();

 private:
  void DoRecognizingAnimationStep();
  void DoWarmingUpAnimationStep();
  void SetImage(const gfx::ImageSkia& image);

  void DrawVolumeOverlay(SkCanvas* canvas,
                         const gfx::ImageSkia& image_skia,
                         float volume);

  
  base::WeakPtrFactory<SpeechRecognitionBubbleBase> weak_factory_;
  int animation_step_;  

  DisplayMode display_mode_;
  base::string16 message_text_;  

  
  scoped_ptr<SkBitmap> mic_image_;
  
  scoped_ptr<SkBitmap> buffer_image_;

  
  int render_process_id_;
  int render_view_id_;

  
  gfx::ImageSkia icon_image_;
  
  float scale_;

  DISALLOW_COPY_AND_ASSIGN(SpeechRecognitionBubbleBase);
};

typedef SpeechRecognitionBubble::Delegate SpeechRecognitionBubbleDelegate;

#endif  
