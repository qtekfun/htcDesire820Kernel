// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_CHROMOTING_MODULE_H_
#define REMOTING_HOST_WIN_CHROMOTING_MODULE_H_

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/win/scoped_com_initializer.h"

#include "remoting/host/chromoting_lib.h"

namespace base {
namespace win {
class ScopedCOMInitializer;
}  
}  

namespace remoting {

class AutoThreadTaskRunner;

class ChromotingModule : public ATL::CAtlModuleT<ChromotingModule> {
 public:
  
  ChromotingModule(ATL::_ATL_OBJMAP_ENTRY* classes,
                   ATL::_ATL_OBJMAP_ENTRY* classes_end);
  virtual ~ChromotingModule();

  
  
  static scoped_refptr<AutoThreadTaskRunner> task_runner();

  
  
  bool Run();

  
  virtual LONG Unlock() OVERRIDE;

  DECLARE_LIBID(LIBID_ChromotingLib)

 private:
  
  HRESULT RegisterClassObjects(DWORD class_context, DWORD flags);
  HRESULT RevokeClassObjects();

  
  base::win::ScopedCOMInitializer com_initializer_;

  
  ATL::_ATL_OBJMAP_ENTRY* classes_;
  ATL::_ATL_OBJMAP_ENTRY* classes_end_;

  DISALLOW_COPY_AND_ASSIGN(ChromotingModule);
};

} 

#endif  
