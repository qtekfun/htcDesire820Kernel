// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_APP_MODE_KIOSK_OEM_MANIFEST_PARSER_H_
#define CHROMEOS_APP_MODE_KIOSK_OEM_MANIFEST_PARSER_H_

#include <string>

#include "base/files/file_path.h"
#include "chromeos/chromeos_export.h"

namespace chromeos {

class CHROMEOS_EXPORT KioskOemManifestParser {
 public:
  
  struct Manifest {
    Manifest();

    
    bool enterprise_managed;
    
    bool can_exit_enrollment;
    
    
    std::string device_requisition;
    
    bool keyboard_driven_oobe;
  };

  
  
  static bool Load(const base::FilePath& kiosk_oem_file,
                   Manifest* manifest);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(KioskOemManifestParser);
};

}  

#endif  
