// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_METRICS_NACL_HISTOGRAM_H_
#define BASE_METRICS_NACL_HISTOGRAM_H_
#pragma once

enum NaClHistogramValue {
  FIRST_TAB_NACL_BASELINE,   
  NEW_TAB_NACL_BASELINE,     
  NACL_STARTED,              
  NACL_MAX_HISTOGRAM         
};

void UmaNaclHistogramEnumeration(NaClHistogramValue histogram_value);

#endif  

