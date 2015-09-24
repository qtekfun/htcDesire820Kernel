// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_MEMORY_PURGER_H_
#define CHROME_BROWSER_MEMORY_PURGER_H_
#pragma once

#include "base/basictypes.h"

class RenderProcessHost;

class MemoryPurger {
 public:
  
  static void PurgeAll();
  static void PurgeBrowser();
  static void PurgeRenderers();
  static void PurgeRendererForHost(RenderProcessHost* host);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(MemoryPurger);
};

#endif  
