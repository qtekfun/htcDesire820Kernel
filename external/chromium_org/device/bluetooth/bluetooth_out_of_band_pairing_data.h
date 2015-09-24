// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_OUT_OF_BAND_PAIRING_DATA_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_OUT_OF_BAND_PAIRING_DATA_H_

#include "base/basictypes.h"

namespace device {

const size_t kBluetoothOutOfBandPairingDataSize = 16;

struct BluetoothOutOfBandPairingData {
  
  uint8 hash[kBluetoothOutOfBandPairingDataSize];

  
  uint8 randomizer[kBluetoothOutOfBandPairingDataSize];
};

}  

#endif  
