// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP2T_TS_SECTION_PAT_H_
#define MEDIA_MP2T_TS_SECTION_PAT_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "media/mp2t/ts_section_psi.h"

namespace media {
namespace mp2t {

class TsSectionPat : public TsSectionPsi {
 public:
  
  typedef base::Callback<void(int, int)> RegisterPmtCb;

  explicit TsSectionPat(const RegisterPmtCb& register_pmt_cb);
  virtual ~TsSectionPat();

  
  virtual bool ParsePsiSection(BitReader* bit_reader) OVERRIDE;
  virtual void ResetPsiSection() OVERRIDE;

 private:
  RegisterPmtCb register_pmt_cb_;

  
  int version_number_;

  DISALLOW_COPY_AND_ASSIGN(TsSectionPat);
};

}  
}  

#endif

