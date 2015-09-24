// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_SOUNDS_SOUNDS_MANAGER_H_
#define MEDIA_AUDIO_SOUNDS_SOUNDS_MANAGER_H_

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/strings/string_piece.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT SoundsManager : public base::NonThreadSafe {
 public:
  typedef int SoundKey;

  
  static void Create();

  
  static void Shutdown();

  
  static SoundsManager* Get();

  
  
  
  virtual bool Initialize(SoundKey key, const base::StringPiece& data) = 0;

  
  
  virtual bool Play(SoundKey key) = 0;

  
  
  
  virtual base::TimeDelta GetDuration(SoundKey key) = 0;

 protected:
  SoundsManager();
  virtual ~SoundsManager();

 private:
  DISALLOW_COPY_AND_ASSIGN(SoundsManager);
};

}  

#endif  
