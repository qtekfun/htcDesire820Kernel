// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_IMMERSIVE_REVEALED_LOCK_H_
#define ASH_WM_IMMERSIVE_REVEALED_LOCK_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"

namespace ash {

class ASH_EXPORT ImmersiveRevealedLock {
 public:
  class ASH_EXPORT Delegate {
   public:
    enum AnimateReveal {
      ANIMATE_REVEAL_YES,
      ANIMATE_REVEAL_NO
    };

    virtual void LockRevealedState(AnimateReveal animate_reveal) = 0;
    virtual void UnlockRevealedState() = 0;

   protected:
    virtual ~Delegate() {}
  };

  ImmersiveRevealedLock(const base::WeakPtr<Delegate>& delegate,
                        Delegate::AnimateReveal animate_reveal);
  ~ImmersiveRevealedLock();

 private:
  base::WeakPtr<Delegate> delegate_;

  DISALLOW_COPY_AND_ASSIGN(ImmersiveRevealedLock);
};

}  

#endif  