// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP2T_TS_SECTION_PMT_H_
#define MEDIA_MP2T_TS_SECTION_PMT_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "media/mp2t/ts_section_psi.h"

namespace media {
namespace mp2t {

class TsSectionPmt : public TsSectionPsi {
 public:
  
  
  
  
  typedef base::Callback<void(int, int)> RegisterPesCb;

  explicit TsSectionPmt(const RegisterPesCb& register_pes_cb);
  virtual ~TsSectionPmt();

  
  virtual bool ParsePsiSection(BitReader* bit_reader) OVERRIDE;
  virtual void ResetPsiSection() OVERRIDE;

 private:
  RegisterPesCb register_pes_cb_;

  DISALLOW_COPY_AND_ASSIGN(TsSectionPmt);
};

}  
}  

#endif

