// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_RENDERER_NET_RENDERER_NET_PREDICTOR_H_
#define CHROME_RENDERER_NET_RENDERER_NET_PREDICTOR_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "chrome/renderer/net/predictor_queue.h"

class RendererNetPredictor {
 public:
  RendererNetPredictor();
  ~RendererNetPredictor();

  
  void Resolve(const char* name, size_t length);

  
  
  
  
  
  
  
  
  void SubmitHostnames();

  
  static bool is_numeric_ip(const char* name, size_t length);

 private:
  
  
  
  
  
  
  
  
  
  void ExtractBufferedNames(size_t size_goal = 0);

  
  
  
  
  void DnsPrefetchNames(size_t max_count = 0);

  
  
  void Reset();

  
  
  
  DnsQueue c_string_queue_;


  
  
  
  static const int kLookupRequested = 0x1;
  static const int kPending = 0x0;
  typedef std::map<std::string, int> DomainUseMap;
  DomainUseMap domain_map_;

  
  
  
  size_t new_name_count_;

  
  
  int buffer_full_discard_count_;
  int numeric_ip_discard_count_;

  base::WeakPtrFactory<RendererNetPredictor> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(RendererNetPredictor);
};  

#endif  
