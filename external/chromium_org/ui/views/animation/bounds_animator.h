// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_ANIMATION_BOUNDS_ANIMATOR_H_
#define UI_VIEWS_ANIMATION_BOUNDS_ANIMATOR_H_

#include <map>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "ui/gfx/animation/animation_container_observer.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/tween.h"
#include "ui/gfx/rect.h"
#include "ui/views/views_export.h"

namespace gfx {
class SlideAnimation;
}

namespace views {

class BoundsAnimatorObserver;
class View;

class VIEWS_EXPORT BoundsAnimator : public gfx::AnimationDelegate,
                                    public gfx::AnimationContainerObserver {
 public:
  
  
  class OwnedAnimationDelegate : public gfx::AnimationDelegate {
   public:
    virtual ~OwnedAnimationDelegate() {}
  };

  explicit BoundsAnimator(View* view);
  virtual ~BoundsAnimator();

  
  
  
  
  void AnimateViewTo(View* view, const gfx::Rect& target);

  
  
  
  void SetTargetBounds(View* view, const gfx::Rect& target);

  
  
  gfx::Rect GetTargetBounds(View* view);

  
  
  void SetAnimationForView(View* view, gfx::SlideAnimation* animation);

  
  
  const gfx::SlideAnimation* GetAnimationForView(View* view);

  
  void StopAnimatingView(View* view);

  
  
  
  void SetAnimationDelegate(View* view,
                            gfx::AnimationDelegate* delegate,
                            bool delete_when_done);

  
  bool IsAnimating(View* view) const;

  
  bool IsAnimating() const;

  
  
  void Cancel();

  
  
  void SetAnimationDuration(int duration_ms);

  
  void set_tween_type(gfx::Tween::Type type) { tween_type_ = type; }

  void AddObserver(BoundsAnimatorObserver* observer);
  void RemoveObserver(BoundsAnimatorObserver* observer);

 protected:
  
  virtual gfx::SlideAnimation* CreateAnimation();

 private:
  
  struct Data {
    Data()
        : delete_delegate_when_done(false),
          animation(NULL),
          delegate(NULL) {}

    
    bool delete_delegate_when_done;

    
    gfx::Rect start_bounds;

    
    gfx::Rect target_bounds;

    
    gfx::SlideAnimation* animation;

    
    gfx::AnimationDelegate* delegate;
  };

  
  enum AnimationEndType {
    ANIMATION_ENDED,
    ANIMATION_CANCELED
  };

  typedef std::map<View*, Data> ViewToDataMap;

  typedef std::map<const gfx::Animation*, View*> AnimationToViewMap;

  
  
  void RemoveFromMaps(View* view);

  
  
  void CleanupData(bool send_cancel, Data* data, View* view);

  
  
  
  gfx::Animation* ResetAnimationForView(View* view);

  
  void AnimationEndedOrCanceled(const gfx::Animation* animation,
                                AnimationEndType type);

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const gfx::Animation* animation) OVERRIDE;

  
  virtual void AnimationContainerProgressed(
      gfx::AnimationContainer* container) OVERRIDE;
  virtual void AnimationContainerEmpty(
      gfx::AnimationContainer* container) OVERRIDE;

  
  View* parent_;

  ObserverList<BoundsAnimatorObserver> observers_;

  
  scoped_refptr<gfx::AnimationContainer> container_;

  
  ViewToDataMap data_;

  
  AnimationToViewMap animation_to_view_;

  
  
  
  
  gfx::Rect repaint_bounds_;

  int animation_duration_ms_;

  gfx::Tween::Type tween_type_;

  DISALLOW_COPY_AND_ASSIGN(BoundsAnimator);
};

}  

#endif  
