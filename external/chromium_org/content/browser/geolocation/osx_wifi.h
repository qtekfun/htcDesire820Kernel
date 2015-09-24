// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


// license with the following restrictions.
// Copyright (c) 02006, Alf Watt (alf@istumbler.net). All rights reserved.
// * Redistributions of source code must retain the above copyright
// * Redistributions in binary form must reproduce the above copyright
//   specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER

#ifndef CONTENT_BROWSER_GEOLOCATION_OSX_WIFI_H_
#define CONTENT_BROWSER_GEOLOCATION_OSX_WIFI_H_

#include <CoreFoundation/CoreFoundation.h>

extern "C" {

typedef SInt32 WIErr;

struct WirelessContext;

typedef WIErr (*WirelessAttachFunction)(WirelessContext** outContext,
                                        const UInt32);

typedef WIErr (*WirelessDetachFunction)(WirelessContext* inContext);

typedef UInt16 WINetworkInfoFlags;

struct WirelessNetworkInfo
{
  UInt16 channel;            
  SInt16 noise;              
  SInt16 signal;             
  UInt8 macAddress[6];       
  UInt16 beaconInterval;     
  WINetworkInfoFlags flags;  
  UInt16 nameLen;
  SInt8 name[32];
};

typedef WIErr (*WirelessScanSplitFunction)(WirelessContext* inContext,
                                           CFArrayRef* apList,
                                           CFArrayRef* adhocList,
                                           const UInt32 stripDups);

}  

#endif  
