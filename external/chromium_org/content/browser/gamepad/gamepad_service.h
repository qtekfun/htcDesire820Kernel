// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GAMEPAD_GAMEPAD_SERVICE_H
#define CONTENT_BROWSER_GAMEPAD_GAMEPAD_SERVICE_H

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/memory/singleton.h"
#include "base/threading/thread_checker.h"
#include "content/common/content_export.h"

namespace content {

class GamepadDataFetcher;
class GamepadProvider;
class GamepadServiceTestConstructor;
class RenderProcessHost;

class CONTENT_EXPORT GamepadService {
 public:
  
  static GamepadService* GetInstance();

  
  
  
  
  void AddConsumer();

  
  
  
  void RemoveConsumer();

  
  
  
  void RegisterForUserGesture(const base::Closure& closure);

  
  
  base::SharedMemoryHandle GetSharedMemoryHandleForProcess(
      base::ProcessHandle handle);

  
  void Terminate();

 private:
  friend struct DefaultSingletonTraits<GamepadService>;
  friend class GamepadServiceTestConstructor;

  GamepadService();

  
  
  GamepadService(scoped_ptr<GamepadDataFetcher> fetcher);

  virtual ~GamepadService();

  int num_readers_;
  scoped_ptr<GamepadProvider> provider_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(GamepadService);
};

}  

#endif  
