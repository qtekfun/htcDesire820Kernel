// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_GMOCK_CALLBACK_SUPPORT_H_
#define MEDIA_BASE_GMOCK_CALLBACK_SUPPORT_H_

#include "testing/gmock/include/gmock/gmock.h"

namespace media {


MATCHER(IsNullCallback, "a null callback") {
  return (arg.is_null());
}

MATCHER(IsNotNullCallback, "a non-null callback") {
  return (!arg.is_null());
}


ACTION_TEMPLATE(RunClosure,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_0_VALUE_PARAMS()) {
  ::std::tr1::get<k>(args).Run();
}


ACTION_TEMPLATE(RunCallback,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_0_VALUE_PARAMS()) {
  return ::std::tr1::get<k>(args).Run();
}

ACTION_TEMPLATE(RunCallback,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_1_VALUE_PARAMS(p0)) {
  return ::std::tr1::get<k>(args).Run(p0);
}

ACTION_TEMPLATE(RunCallback,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_2_VALUE_PARAMS(p0, p1)) {
  return ::std::tr1::get<k>(args).Run(p0, p1);
}

ACTION_TEMPLATE(RunCallback,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_3_VALUE_PARAMS(p0, p1, p2)) {
  return ::std::tr1::get<k>(args).Run(p0, p1, p2);
}

ACTION_TEMPLATE(RunCallback,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_4_VALUE_PARAMS(p0, p1, p2, p3)) {
  return ::std::tr1::get<k>(args).Run(p0, p1, p2, p3);
}

ACTION_TEMPLATE(RunCallback,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_5_VALUE_PARAMS(p0, p1, p2, p3, p4)) {
  return ::std::tr1::get<k>(args).Run(p0, p1, p2, p3, p4);
}

ACTION_TEMPLATE(RunCallback,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_6_VALUE_PARAMS(p0, p1, p2, p3, p4, p5)) {
  return ::std::tr1::get<k>(args).Run(p0, p1, p2, p3, p4, p5);
}

ACTION_TEMPLATE(RunCallback,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_7_VALUE_PARAMS(p0, p1, p2, p3, p4, p5, p6)) {
  return ::std::tr1::get<k>(args).Run(p0, p1, p2, p3, p4, p5, p6);
}

}  

#endif  
