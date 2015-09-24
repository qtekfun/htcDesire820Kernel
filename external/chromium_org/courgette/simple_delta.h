// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef COURGETTE_SIMPLE_DELTA_H_
#define COURGETTE_SIMPLE_DELTA_H_

#include "courgette/courgette.h"
#include "courgette/streams.h"

namespace courgette {

Status ApplySimpleDelta(SourceStream* old, SourceStream* delta,
                        SinkStream* target);

Status GenerateSimpleDelta(SourceStream* old, SourceStream* target,
                           SinkStream* delta);

}  

#endif  
