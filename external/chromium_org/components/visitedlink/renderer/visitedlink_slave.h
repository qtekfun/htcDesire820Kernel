// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VISITEDLINK_RENDERER_VISITEDLINK_SLAVE_H_
#define COMPONENTS_VISITEDLINK_RENDERER_VISITEDLINK_SLAVE_H_

#include "base/compiler_specific.h"
#include "base/memory/shared_memory.h"
#include "components/visitedlink/common/visitedlink_common.h"
#include "content/public/renderer/render_process_observer.h"

namespace visitedlink {

class VisitedLinkSlave : public VisitedLinkCommon,
                         public content::RenderProcessObserver {
 public:
  VisitedLinkSlave();
  virtual ~VisitedLinkSlave();

  
  virtual bool OnControlMessageReceived(const IPC::Message& message) OVERRIDE;

  
  void OnUpdateVisitedLinks(base::SharedMemoryHandle table);
  void OnAddVisitedLinks(const VisitedLinkSlave::Fingerprints& fingerprints);
  void OnResetVisitedLinks();

 private:
  void FreeTable();

  
  base::SharedMemory* shared_memory_;

  DISALLOW_COPY_AND_ASSIGN(VisitedLinkSlave);
};

}  

#endif  
