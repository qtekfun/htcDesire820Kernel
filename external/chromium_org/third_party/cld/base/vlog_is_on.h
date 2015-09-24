// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_VLOG_IS_ON_H_
#define BASE_VLOG_IS_ON_H_

#include "base/atomicops.h"
#include "base/basictypes.h"
#include "base/port.h"
#include "base/commandlineflags.h"
#include "base/log_severity.h"

DECLARE_int32(v);  
DECLARE_bool(silent_init);  

#if defined(__GNUC__)
#define VLOG_IS_ON(verboselevel)                                               \
  ({ static Atomic32 site__ = ::base::internal::kDefaultSite;                  \
     ::base::internal::VLogEnabled(&site__, (verboselevel), __FILE__); })
#else
#define VLOG_IS_ON(verboselevel)                                               \
  (FLAGS_v >= (verboselevel) + FLAGS_silent_init)
#endif

int SetVLOGLevel(const char* module_pattern, int log_level);

namespace base {
namespace internal {

extern Atomic32 vlog_epoch;

const int kUseFlag = -1;

const Atomic32 kDefaultSite = kUseFlag << 16;

inline Atomic32 GlobalEpoch() {
  return Acquire_Load(&vlog_epoch) & 0x0000FFFF;
}

inline int SiteEpoch(Atomic32 site) { return site & 0x0000FFFF; }

inline int SiteLevel(Atomic32 site) { return site >> 16; }

inline Atomic32 Site(int level, int epoch) {
  return ((level & 0x0000FFFF) << 16) | (epoch & 0x0000FFFF);
}

int InitVLOG(Atomic32* site, const char* fname);

inline bool VLogEnabled(Atomic32* site, int32 level, const char* const file) {
  const Atomic32 site_copy = Acquire_Load(site);
  const int32 site_level =
      PREDICT_TRUE(SiteEpoch(site_copy) == GlobalEpoch()) ?
      SiteLevel(site_copy) : InitVLOG(site, file);
  return (site_level == kUseFlag ? FLAGS_v : site_level) >=
      (level + FLAGS_silent_init);
}

}  
}  

#endif  
